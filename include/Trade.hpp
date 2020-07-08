/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** Trade
*/

#ifndef TRADE_HPP_
#define TRADE_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <iomanip>
#include <ios>
#include <map>
#include <bits/stdc++.h>

#include "TradeStruct.hpp"

class Trade {

    public:
        Trade();
        ~Trade();
        void core();
        void checkParam();
        void settings();
        void update();
        void action();
        bool buyMoney(std::vector<candle_t>, std::string, float, float, bool);
    protected:
    private:
        std::string _input;
        std::vector<std::string> _data;
        settings_t _settings;

        std::vector<int> _format;
        std::vector<int> formatCandle();
        std::map<std::string, std::vector<candle_t>> _crypto;
        std::map<std::string, float> _value;
};

void trade(void);

#endif /* !TRADE_HPP_ */
