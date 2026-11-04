#ifndef FINANCE_H
#define FINANCE_H

#include "status.h"

typedef struct {
    double *cash_flows;
    int periods;
} irr_ctx;

int fin_npv(double rate, double *cash_flows, int periods, double *result);
int fin_irr(double *cash_flows, int periods, double *result);
int fin_french_payment(double principal, double annual_rate, int months, double *result);
int fin_french_amortization(double principal, double annual_rate, int months, int period, double *interest, double *principal_paid);
int fin_german_amortization(double principal, double annual_rate, int months, int period, double *interest, double *principal_paid);
int fin_rate_nominal_to_effective(double nominal, int compounding_periods, double *result);
int fin_rate_effective_to_nominal(double effective, int compounding_periods, double *result);
int fin_rate_continuous(double effective, double *result);
int fin_deprec_linear(double cost, double salvage, int life, int year, double *result);
int fin_deprec_ddb(double cost, double salvage, int life, int year, double *result);
int fin_deprec_syd(double cost, double salvage, int life, int year, double *result);

#endif
