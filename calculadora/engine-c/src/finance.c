#include "finance.h"
#include "numeric.h"
#include <math.h>
#include <stdlib.h>

static double npv_callback(double rate, void *ctx) {
    irr_ctx *data = (irr_ctx *)ctx;
    double npv = 0.0;
    for (int i = 0; i < data->periods; i++) {
        npv += data->cash_flows[i] / pow(1.0 + rate, i);
    }
    return npv;
}

int fin_npv(double rate, double *cash_flows, int periods, double *result) {
    if (!cash_flows || !result) return ENG_ERR_ARG;
    double npv = 0.0;
    for (int i = 0; i < periods; i++) {
        npv += cash_flows[i] / pow(1.0 + rate, i);
    }
    *result = npv;
    return ENG_OK;
}

int fin_irr(double *cash_flows, int periods, double *result) {
    if (!cash_flows || !result) return ENG_ERR_ARG;
    irr_ctx ctx = { .cash_flows = cash_flows, .periods = periods };
    int iters = 0;
    return num_root_brent(npv_callback, &ctx, -0.99, 10.0, 1e-7, 100, result, &iters);
}

int fin_french_payment(double principal, double annual_rate, int months, double *result) {
    if (!result || months <= 0) return ENG_ERR_ARG;
    double monthly_rate = annual_rate / 12.0;
    if (monthly_rate == 0) {
        *result = principal / months;
        return ENG_OK;
    }
    *result = (principal * monthly_rate) / (1.0 - pow(1.0 + monthly_rate, -months));
    return ENG_OK;
}

int fin_french_amortization(double principal, double annual_rate, int months, int period, double *interest, double *principal_paid) {
    if (!interest || !principal_paid || period < 1 || period > months) return ENG_ERR_ARG;
    double monthly_rate = annual_rate / 12.0;
    double pmt_val;
    fin_french_payment(principal, annual_rate, months, &pmt_val);
    double s_prev = principal * (pow(1.0+monthly_rate, months) - pow(1.0+monthly_rate, period-1)) / (pow(1.0+monthly_rate, months)-1);
    if(period == 1) s_prev = principal;
    *interest = s_prev * monthly_rate;
    *principal_paid = pmt_val - *interest;
    return ENG_OK;
}

int fin_german_amortization(double principal, double annual_rate, int months, int period, double *interest, double *principal_paid) {
    if (!interest || !principal_paid || period < 1 || period > months) return ENG_ERR_ARG;
    double monthly_rate = annual_rate / 12.0;
    *principal_paid = principal / months;
    *interest = (principal - (*principal_paid * (period - 1))) * monthly_rate;
    return ENG_OK;
}

int fin_rate_nominal_to_effective(double nominal, int compounding_periods, double *result) {
    if (!result || compounding_periods <= 0) return ENG_ERR_ARG;
    *result = pow(1.0 + (nominal / compounding_periods), compounding_periods) - 1.0;
    return ENG_OK;
}

int fin_rate_effective_to_nominal(double effective, int compounding_periods, double *result) {
    if (!result || compounding_periods <= 0) return ENG_ERR_ARG;
    *result = compounding_periods * (pow(1.0 + effective, 1.0 / compounding_periods) - 1.0);
    return ENG_OK;
}

int fin_rate_continuous(double effective, double *result) {
    if (!result) return ENG_ERR_ARG;
    *result = log(1.0 + effective);
    return ENG_OK;
}

int fin_deprec_linear(double cost, double salvage, int life, int year, double *result) {
    if (!result || life <= 0 || year < 1 || year > life) return ENG_ERR_ARG;
    *result = (cost - salvage) / life;
    return ENG_OK;
}

int fin_deprec_ddb(double cost, double salvage, int life, int year, double *result) {
    if (!result || life <= 0 || year < 1 || year > life) return ENG_ERR_ARG;
    double factor = 2.0;
    double rate = factor / life;
    double current_value = cost;
    double depreciation = 0;
    for (int i = 1; i <= year; i++) {
        depreciation = current_value * rate;
        if (current_value - depreciation < salvage) {
            depreciation = current_value - salvage;
        }
        current_value -= depreciation;
    }
    *result = depreciation;
    return ENG_OK;
}

int fin_deprec_syd(double cost, double salvage, int life, int year, double *result) {
    if (!result || life <= 0 || year < 1 || year > life) return ENG_ERR_ARG;
    double sum_years = (life * (life + 1)) / 2.0;
    *result = (cost - salvage) * (life - year + 1) / sum_years;
    return ENG_OK;
}
