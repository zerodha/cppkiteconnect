/*
 *  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 *  SPDX-License-Identifier: MIT
 *
 *  Copyright (c) 2020-2021 Bhumit Attarde
 *
 *  Permission is hereby  granted, free of charge, to any  person obtaining a
 * copy of this software and associated  documentation files (the "Software"),
 * to deal in the Software  without restriction, including without  limitation
 * the rights to  use, copy,  modify, merge,  publish, distribute,  sublicense,
 * and/or  sell copies  of  the Software,  and  to  permit persons  to  whom the
 * Software  is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS
 * OR IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN
 * NO EVENT  SHALL THE AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY
 * CLAIM,  DAMAGES OR  OTHER LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

/**
 * @file userconstants.hpp
 * @brief this file has useful constants that users can use
 */

#include <string>

namespace kiteconnect {
// NOLINTBEGIN(cert-err58-cpp)

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

// kiteWS modes
const string MODE_LTP = "ltp";
const string MODE_QUOTE = "quote";
const string MODE_FULL = "full";

// NOLINTEND(cert-err58-cpp)
} // namespace kiteconnect
