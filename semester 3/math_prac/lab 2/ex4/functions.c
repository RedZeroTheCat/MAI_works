#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

enum input_status_code {
    isc_invalid,
    isc_dot,
    isc_line,
    isc_triangle,
    isc_convex,
    isc_not_convex
};

struct cord {
    long double x;
    long double y;
};

enum input_status_code analyzer(enum input_status_code is_convex) {
    switch (is_convex) {
        case isc_invalid:
            printf("Incorrect input.\n");
            break;
        case isc_dot:
            printf("Entered only one coordinate, it is dot.\n");
            break;
        case isc_line:
            printf("Entered only two coordinates, it is line.\n");
            break;
        case isc_triangle:
            printf("Polygon is triangle, which is convex.\n");
            break;
        case isc_convex:
            printf("Polygon is convex.\n");
            break;
        case isc_not_convex:
            printf("Polygon is not convex.\n");
            break;
    }
}

enum input_status_code is_convex(int count, ...) {
    if (count == 0) {
        return isc_invalid;
    }
    if (count == 1) {
        return isc_dot;
    }
    if (count == 2) {
        return isc_line;
    }
    if (count == 3) {
        return isc_triangle;
    }
    va_list argv;
    va_start(argv, count);
    struct cord prev, cur, next;
    prev = va_arg(argv, struct cord);
    cur = va_arg(argv, struct cord);
    next = va_arg(argv, struct cord);

    // направление обхода:
    // 0 - не определено,
    // 1 - по часовой,
    // -1 - против часовой.
    int bypass_dir = 0;
    int same_x = 1, same_y = 1;

    for (int i = 3; i <= count; i++) {
        struct cord edge_prev, edge_next;
        edge_prev.x = cur.x - prev.x;
        edge_prev.y = cur.y - prev.y;
        edge_next.x = next.x - cur.x;
        edge_next.y = next.y - cur.y;

        if (edge_next.x == edge_prev.x) {
            cur = next;
            next = va_arg(argv, struct cord);
            continue;
        }
        else {
            same_x = 0;
        }

        if (edge_next.y == edge_prev.y) {
            cur = next;
            next = va_arg(argv, struct cord);
            continue;
        }
        else {
            same_y = 0;
        }

        long double is_const_dir = edge_prev.x * edge_next.y - edge_prev.y * edge_next.x;
        if (is_const_dir != 0) {
            if (bypass_dir == 0) {
                if (is_const_dir > 0) {
                    bypass_dir = 1;
                }
                else {
                    bypass_dir = -1;
                }
            }
            else if ((is_const_dir > 0 && bypass_dir == -1) ||
                     (is_const_dir <= 0 && bypass_dir == 1)) {
                va_end(argv);
                return isc_not_convex;
            }
        }
        prev = cur;
        cur = next;
        next = va_arg(argv, struct cord);
    }
    if (same_x || same_y) {
        va_end(argv);
        return isc_not_convex;
    }
    va_end(argv);
    return isc_convex;
}

double polynom_value(double x, bool* pos_grade, int n, ...) {
    if (n < 0) {
        *pos_grade = false;
        return NAN;
    }
    va_list coef;
    va_start(coef, n);
    double result = 0;
    for(int i = 0; i < n + 1; i++)
    {
        result *= x;
        result += va_arg(coef, double);
    }
    va_end(coef);
    return result;
}
