#ifndef lab1_1_enum_h
#define lab1_1_enum_h

enum input_status_code {
    isc_invalid_argument,
    isc_no_number_input,
    isc_asking_for_help,
    isc_too_many_arguments,
    isc_negative_number_input,
    isc_h,
    isc_p,
    isc_s,
    isc_e,
    isc_a,
    isc_f
};

enum plain_status_code {
    psc_negative,
    psc_zero,
    psc_one,
    psc_simple,
    psc_complicated
};

enum string_status_code {
    ssc_wrong_input,
    ssc_correct_input
};

enum extent_status_code {
    esc_out_of_bounds,
    esc_correct_input
};

enum algebra_status_code {
    asc_zero,
    asc_negative,
    asc_correct_input
}; 

enum factorial_status_code {
    fsc_ok,
    fsc_invalid_parameter,
};

#endif