/*
** EPITECH PROJECT, 2020
** Trade Project
** File description:
** nathan.bellanger & matthieu.vetois
*/

#include "Trade.hpp"

Trade::Trade()
{
}

Trade::~Trade()
{
}

template<typename T>
static void pop_front(std::vector<T> &v)
{
    if (v.size() > 0)
        v.erase(v.begin());
}

static int typeId(const char *str)
{
    if (std::strcmp(str, "pair") == 0)
        return (0);
    else if (std::strcmp(str, "date") == 0)
        return (1);
    else if (std::strcmp(str, "high") == 0)
        return (2);
    else if (std::strcmp(str, "low") == 0)
        return (3);
    else if (std::strcmp(str, "open") == 0)
        return (4);
    else if (std::strcmp(str, "close") == 0)
        return (5);
    else if (std::strcmp(str, "volume") == 0)
        return (6);
    return (-1);
}

std::vector<int> Trade::formatCandle()
{
    std::vector<int> tmpV;
    char *tmp = const_cast<char *>(_settings.candle_format.c_str());
    char *token = std::strtok(tmp, ",");

    while (token != NULL) {
        tmpV.push_back(typeId(token));
        token = std::strtok(NULL, ",");
    }
    return (tmpV);
}

void Trade::settings()
{
    pop_front(_data);
    if (std::strcmp(_data.front().c_str(), "player_names") == 0)
        _settings.player_name = _data.back().c_str();
    else if (std::strcmp(_data.front().c_str(), "your_bot") == 0)
        _settings.your_bot = _data.back().c_str();
    else if (std::strcmp(_data.front().c_str(), "timebank") == 0)
        _settings.timebank = std::atoi(_data.back().c_str());
    else if (std::strcmp(_data.front().c_str(), "time_per_move") == 0)
        _settings.time_per_move = std::atoi(_data.back().c_str());
    else if (std::strcmp(_data.front().c_str(), "candle_interval") == 0)
        _settings.candle_interval = std::atoi(_data.back().c_str());
    else if (std::strcmp(_data.front().c_str(), "candle_format") == 0) {
        _settings.candle_format = _data.back();
        _format = formatCandle();
    } else if (std::strcmp(_data.front().c_str(), "candles_total") == 0)
        _settings.candles_total = std::atoi(_data.back().c_str());
    else if (std::strcmp(_data.front().c_str(), "candles_given") == 0)
        _settings.candles_given = std::atoi(_data.back().c_str());
    else if (std::strcmp(_data.front().c_str(), "initial_stack") == 0)
        _settings.initial_stack = std::atoi(_data.back().c_str());
    else if (std::strcmp(_data.front().c_str(), "transaction_fee_percent") == 0)
        _settings.transaction_fee_percent = std::atof(_data.back().c_str());
}


void Trade::update()
{
    pop_front(_data);
    if (std::strcmp(_data.front().c_str(), "game") != 0)
        return;
    pop_front(_data);
    if (std::strcmp(_data.front().c_str(), "next_candles") == 0) {
        std::vector<std::string> tmpV;
        char *tmp = const_cast<char *>(_data.back().c_str());
        char *token = std::strtok(tmp, ";");

        while (token != NULL) {
            tmpV.push_back(token);
            token = std::strtok(NULL, ";");
        }
        for (int count = 0; count < tmpV.size(); count++) {
            std::vector<std::string> tmpVB;
            tmp = const_cast<char *>(tmpV[count].c_str());
            token = std::strtok(tmp, ",");

            while (token != NULL) {
                tmpVB.push_back(token);
                token = std::strtok(NULL, ",");
            }
            candle_t t;
            for (int j = 1; j < tmpVB.size(); j++) {
                if (_format[j] == 1)
                    t.date = std::atoi(tmpVB[j].c_str());
                else if (_format[j] == 2)
                    t.high = std::atof(tmpVB[j].c_str());
                else if (_format[j] == 3)
                    t.low = std::atof(tmpVB[j].c_str());
                else if (_format[j] == 4)
                    t.open = std::atof(tmpVB[j].c_str());
                else if (_format[j] == 5)
                    t.close = std::atof(tmpVB[j].c_str());
                else if (_format[j] == 6)
                    t.volume = std::atof(tmpVB[j].c_str());
            }
            bool isFirstTime = true;
            for (std::map<std::string, std::vector<candle_t>>::iterator it = _crypto.begin(); it != _crypto.end(); it++) {
                if (it->first == tmpVB.front()) {
                    it->second.push_back(t);
                    isFirstTime = false;
                }
            }
            if (isFirstTime) {
                std::vector<candle_t> tempo;
                tempo.push_back(t);
                _crypto.insert(std::make_pair(tmpVB.front(), tempo));
            }
        }
    } else if (std::strcmp(_data.front().c_str(), "stacks") == 0) {
        pop_front(_data);
        std::vector<std::string> tmpV;
        char *tmp = const_cast<char *>(_data.back().c_str());
        char *token = std::strtok(tmp, ",");

        while (token != NULL) {
            tmpV.push_back(token);
            token = std::strtok(NULL, ",");
        }
        for (int i = 0; i < tmpV.size(); i++) {
            std::vector<std::string> tmpVB;
            tmp = const_cast<char *>(tmpV[i].c_str());
            token = std::strtok(tmp, ":");

            while (token != NULL) {
                tmpVB.push_back(token);
                token = std::strtok(NULL, ":");
            }
            bool isFirstTime = true;
            for (std::map<std::string, float>::iterator it = _value.begin(); it != _value.end(); it++) {
                if (it->first == tmpVB.front()) {
                    it->second = std::atof(tmpVB[1].c_str());
                    isFirstTime = false;
                }
            }
            if (isFirstTime) {
                _value.insert(std::make_pair(tmpVB.front(), std::atof(tmpVB[1].c_str())));
            }
        }
    }
}

bool Trade::buyMoney(std::vector<candle_t> vc, std::string name, float moneyStack, float sellMoneyStack, bool notFirstBougth)
{
    // Vector of all close value but not the last one
    std::vector<float> closeArr;
    for (size_t i = 0; i < vc.size() - 2; i++)
        closeArr.push_back(vc[i].close);

    // Get Middle Band
    float middleBB = 0;
    for (int i = vc.size() - 1; i > vc.size() - 1 - _settings.candles_given; i--)
        middleBB += closeArr[i];
    middleBB /= _settings.candles_given;

    // Standard Deviation
    float stdDev = 0;
    for (int i = vc.size() - 1; i > vc.size() - 1 - _settings.candles_given; i--)
        stdDev += pow(abs(closeArr[i] - middleBB), 2);
    stdDev = sqrt(stdDev / _settings.candles_given);

    float lowBB = middleBB - (2 * stdDev);
    float highBB = middleBB + (2 * stdDev);
    float buyValue = ((lowBB - closeArr.back()) / 10) * moneyStack;
    float sellValues = ((closeArr.back() - highBB) / 10) * sellMoneyStack;

    // Check if you must buy or sell
    if (closeArr.back() < lowBB && moneyStack > buyValue && buyValue > 0.001) {
        if (notFirstBougth)
            std::cout << ";";
        std::cout << "buy " << name << " " << buyValue;
        return (true);
    } else if (closeArr.back() > highBB && sellMoneyStack > sellValues && sellValues > 0.5) {
        if (notFirstBougth)
            std::cout << ";";
        std::cout << "sell " << name << " " << sellValues;
        return (true);
    }
    return (false);
}

void Trade::action()
{
    bool passBool = false;
    bool hasBought = buyMoney(_crypto["USDT_ETH"], "USDT_ETH", _value["USDT"] / _crypto["USDT_ETH"][_crypto["USDT_ETH"].size() - 2].close, _value["ETH"], false);
    passBool = hasBought || passBool;
    hasBought = buyMoney(_crypto["BTC_ETH"], "BTC_ETH", _value["BTC"] / _crypto["BTC_ETH"][_crypto["BTC_ETH"].size() - 2].close, _value["ETH"], passBool);
    passBool = hasBought || passBool;
    hasBought = buyMoney(_crypto["USDT_BTC"], "USDT_BTC", _value["USDT"] / _crypto["USDT_BTC"][_crypto["USDT_BTC"].size() - 2].close, _value["BTC"], passBool);
    passBool = hasBought || passBool;
    if (!passBool)
        std::cout << "pass" << std::endl;
    else
        std::cout << std::endl;
}

void Trade::checkParam()
{
    if (std::strcmp(_data.front().c_str(), "settings") == 0)
        settings();
    else if (std::strcmp(_data.front().c_str(), "update") == 0)
        update();
    else if (std::strcmp(_data.front().c_str(), "action") == 0)
        action();
}

void Trade::core()
{
    while (1) {
        if (!std::getline(std::cin, _input))
            exit(84);
        char *inputChar = const_cast<char *>(_input.c_str());
        char *tmp = std::strtok(inputChar, " ");
        while (tmp != NULL) {
            _data.push_back(tmp);
            tmp = std::strtok(NULL, " ");
        }
        if (std::strcmp(_data[0].c_str(), "end") == 0)
            break;
        checkParam();
        _data.clear();
    }
}

void trade(void)
{
    Trade *a = new Trade;

    a->core();
}
