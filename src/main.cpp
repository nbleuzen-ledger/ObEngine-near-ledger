#include <Engine/Engine.hpp>

#include <PluginSDK.hpp>

#include <ledgercpp/ledger.hpp>

#include <algorithm>
#include <iostream>
#include <string>

std::vector<std::string> split(const std::string& str, const std::string& delimiters)
{
    std::vector<std::string> tokens;
    std::string::size_type last_pos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, last_pos);
    while (std::string::npos != pos || std::string::npos != last_pos)
    {
        tokens.push_back(str.substr(last_pos, pos - last_pos));
        last_pos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, last_pos);
    }
    return tokens;
}

bool ends_with(const std::string& string, const std::string& search)
{
    if (string.size() < search.size())
    {
        return false;
    }
    return (
        std::mismatch(search.rbegin(), search.rend(), string.rbegin()).first == search.rend());
}

bool is_string_int(const std::string& str)
{
    if (!str.empty())
    {
        return std::all_of(str.begin(), str.end(), isdigit);
    }
    return false;
}

std::vector<uint8_t> bip32_path_to_bytes(std::string bip32_path, bool prepend_length = false) {
    constexpr uint32_t HARDENED_FLAG = 0x80000000;

    std::transform(bip32_path.begin(), bip32_path.end(), bip32_path.begin(),
        [](unsigned char c) { return std::tolower(c); });
    if (bip32_path.substr(0, 2) == "m/")
    {
        bip32_path = bip32_path.substr(2);
    }

    const std::vector<std::string> path_parts = split(bip32_path, "/");
    std::vector<uint8_t> bytes;
    bytes.reserve(path_parts.size() * 4 + ((prepend_length) ? 1 : 0));
    if (prepend_length)
    {
        bytes.push_back(path_parts.size());
    }
    for (std::string part : path_parts)
    {
        bool hardened = false;
        if (ends_with(part, "'"))
        {
            hardened = true;
            part = part.substr(0, part.size() - 1);
        }
        if (!is_string_int(part))
        {
            throw std::runtime_error("invalid bip32 path");
        }
        uint32_t part_as_uint32 = std::stoul(part);
        if (hardened)
        {
            part_as_uint32 += HARDENED_FLAG;
        }
        bytes.push_back(static_cast<uint8_t>(part_as_uint32 >> 24));
        bytes.push_back(static_cast<uint8_t>(part_as_uint32 >> 16));
        bytes.push_back(static_cast<uint8_t>(part_as_uint32 >> 8));
        bytes.push_back(static_cast<uint8_t>(part_as_uint32));
    }
    return bytes;
}

ledger::Ledger nano;

std::vector<uint8_t> get_version()
{
    std::tuple<ledger::Error, std::vector<uint8_t>> ret;

    ret = nano.get_version();

    ledger::Error err = std::get<0>(ret);
    std::cout << __func__ << ": " << ledger::error_message(err) << std::endl;

    return std::get<1>(ret);
}

std::vector<uint8_t> get_public_key(std::vector<uint8_t> & account)
{
    std::tuple<ledger::Error, std::vector<uint8_t>> ret;

    ret = nano.get_public_key(account);

    ledger::Error err = std::get<0>(ret);
    std::cout << __func__ << ": " << ledger::error_message(err) << std::endl;

    return std::get<1>(ret);
}

std::vector<uint8_t> sign(std::vector<uint8_t> & account, std::vector<uint8_t> & serialized_tx)
{
    std::tuple<ledger::Error, std::vector<uint8_t>> ret;

    ret = nano.sign(account, serialized_tx, /*p1==P1_LAST*/0x80);

    ledger::Error err = std::get<0>(ret);
    std::cout << __func__ << ": " << ledger::error_message(err) << std::endl;

    return std::get<1>(ret);
}

PLUGIN_FUNC void OnInit(obe::engine::Engine& engine)
{
    const obe::debug::Logger logger = engine.get_logger();
    ledger::Error err = nano.open();
    if (err != ledger::Error::SUCCESS)
        logger->error(ledger::error_message(err));

    sol::state_view state = engine.get_lua_state();
    auto ledger_namespace = state["ledger"].get_or_create<sol::table>();
    ledger_namespace.set_function("bip32_path_to_bytes", &bip32_path_to_bytes);
    ledger_namespace.set_function("get_version", &get_version);
    ledger_namespace.set_function("get_public_key", &get_public_key);
    ledger_namespace.set_function("sign", &sign);
}

PLUGIN_FUNC void OnExit(obe::engine::Engine& engine)
{
    nano.close();
}
