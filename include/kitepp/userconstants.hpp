/*
 *   Copyright (c) 2020 Bhumit Attarde

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/**
 * @file userconstants.hpp
 * @brief this file has useful constants that users can use
 */

#include <string>

namespace kitepp
{

using std::string;

// Products
const string PRODUCT_MIS = "MIS";
const string PRODUCT_CNC = "CNC";
const string PRODUCT_NRML = "NRML";
const string PRODUCT_CO = "CO";
const string PRODUCT_BO = "BO";

// Order types
const string ORDER_TYPE_MARKET = "MARKET";
const string ORDER_TYPE_LIMIT = "LIMIT";
const string ORDER_TYPE_SLM = "SL-M";
const string ORDER_TYPE_SL = "SL";

// Varities
const string VARIETY_REGULAR = "regular";
const string VARIETY_BO = "bo";
const string VARIETY_CO = "co";
const string VARIETY_AMO = "amo";

// Transaction type
const string TRANSACTION_TYPE_BUY = "BUY";
const string TRANSACTION_TYPE_SELL = "SELL";

// Validity
const string VALIDITY_DAY = "DAY";
const string VALIDITY_IOC = "IOC";

// Exchanges
const string EXCHANGE_NSE = "NSE";
const string EXCHANGE_BSE = "BSE";
const string EXCHANGE_NFO = "NFO";
const string EXCHANGE_CDS = "CDS";
const string EXCHANGE_BFO = "BFO";
const string EXCHANGE_MCX = "MCX";

// Margins segments
const string MARGIN_EQUITY = "equity";
const string MARGIN_COMMODITY = "commodity";

// Status constants
const string STATUS_COMPLETE = "COMPLETE";
const string STATUS_REJECTED = "REJECTED";
const string STATUS_CANCELLED = "CANCELLED";

// GTT order type
const string GTT_TYPE_OCO = "two-leg";
const string GTT_TYPE_SINGLE = "single";

// GTT order status
const string GTT_STATUS_ACTIVE = "active";
const string GTT_STATUS_TRIGGERED = "triggered";
const string GTT_STATUS_DISABLED = "disabled";
const string GTT_STATUS_EXPIRED = "expired";
const string GTT_STATUS_CANCELLED = "cancelled";
const string GTT_STATUS_REJECTED = "rejected";
const string GTT_STATUS_DELETED = "deleted";

} // namespace kitepp
