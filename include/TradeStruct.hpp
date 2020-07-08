/*
** EPITECH PROJECT, 2020
** Visual Studio Live Share (Workspace)
** File description:
** TradeStruct
*/

#ifndef TRADESTRUCT_HPP_
#define TRADESTRUCT_HPP_

#include <map>
#include <vector>
#include <string>

typedef struct settings_s {
    std::string player_name;
    std::string your_bot;
    size_t timebank;
    size_t time_per_move;
    size_t candle_interval;
    std::string candle_format;
    size_t candles_total;
    size_t candles_given;
    size_t initial_stack;
    float transaction_fee_percent;
} settings_t;

typedef struct candle_s {
    size_t date;
    float high;
    float low;
    float open;
    float close;
    float volume;
} candle_t;

#endif /* !TRADESTRUCT_HPP_ */
