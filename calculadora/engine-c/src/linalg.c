#include "linalg.h"
#include <math.h>

int la_add(const double *a, const double *b, int r, int c, double *out) {
    if (!a || !b || !out || r <= 0 || c <= 0) return ENG_ERR_ARG;
    for (int i = 0; i < r * c; i++) out[i] = a[i] + b[i];
    return ENG_OK;
}

int la_sub(const double *a, const double *b, int r, int c, double *out) {
    if (!a || !b || !out || r <= 0 || c <= 0) return ENG_ERR_ARG;
    for (int i = 0; i < r * c; i++) out[i] = a[i] - b[i];
    return ENG_OK;
}

int la_mul(const double *a, int r1, int c1, const double *b, int c2, double *out) {
    if (!a || !b || !out || r1 <= 0 || c1 <= 0 || c2 <= 0) return ENG_ERR_ARG;
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            double sum = 0.0;
            for (int k = 0; k < c1; k++) {
                sum += a[i * c1 + k] * b[k * c2 + j];
            }
            out[i * c2 + j] = sum;
        }
    }
    return ENG_OK;
}

int la_transpose(const double *a, int r, int c, double *out) {
    if (!a || !out || r <= 0 || c <= 0) return ENG_ERR_ARG;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            out[j * r + i] = a[i * c + j];
        }
    }
    return ENG_OK;
}

int la_det(const double *a, int n, double *det) {
    if (!a || !det || n <= 0 || n > LA_MAX) return ENG_ERR_ARG;
    double m[LA_MAX * LA_MAX];
    for (int i = 0; i < n * n; i++) m[i] = a[i];
    
    double d = 1.0;
    for (int i = 0; i < n; i++) {
        // Pivoteo parcial
        int pivot = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(m[j * n + i]) > fabs(m[pivot * n + i])) pivot = j;
        }
        if (fabs(m[pivot * n + i]) < 1e-12) { *det = 0.0; return ENG_OK; }
        
        if (pivot != i) {
            for (int j = 0; j < n; j++) {
                double tmp = m[i * n + j];
                m[i * n + j] = m[pivot * n + j];
                m[pivot * n + j] = tmp;
            }
            d = -d;
        }
        
        d *= m[i * n + i];
        for (int j = i + 1; j < n; j++) {
            double factor = m[j * n + i] / m[i * n + i];
            for (int k = i + 1; k < n; k++) {
                m[j * n + k] -= factor * m[i * n + k];
            }
        }
    }
    *det = d;
    return ENG_OK;
}

int la_inv(const double *a, int n, double *out) {
    if (!a || !out || n <= 0 || n > LA_MAX) return ENG_ERR_ARG;
    double aug[LA_MAX][LA_MAX * 2];
    
    // Crear matriz aumentada [A | I]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = a[i * n + j];
            aug[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }
    
    // Gauss-Jordan
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(aug[j][i]) > fabs(aug[pivot][i])) pivot = j;
        }
        if (fabs(aug[pivot][i]) < 1e-12) return ENG_ERR_SINGULAR;
        
        if (pivot != i) {
            for (int j = 0; j < 2 * n; j++) {
                double tmp = aug[i][j];
                aug[i][j] = aug[pivot][j];
                aug[pivot][j] = tmp;
            }
        }
        
        double div = aug[i][i];
        for (int j = 0; j < 2 * n; j++) aug[i][j] /= div;
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double factor = aug[j][i];
                for (int k = 0; k < 2 * n; k++) {
                    aug[j][k] -= factor * aug[i][k];
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            out[i * n + j] = aug[i][j + n];
        }
    }
    return ENG_OK;
}
