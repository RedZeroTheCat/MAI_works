#include <stdio.h>

typedef enum {
    correct,
    invalid_input,
    error_file
} status_code;

status_code validate_input(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid input\n");
        return invalid_input;
    }
    return correct;
}

status_code try_to_open(FILE** file, char* path, char* param) {
    *file = fopen(path, param);
    if (file == NULL) {
        printf("Invalid file path, file was not opened\n");
        return error_file;
    }
    return correct;
}

status_code fwrite_task_file(FILE *output) {
    char byte_massive[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fwrite(byte_massive, sizeof(char),  11, output);
    return correct;
}

status_code fread_task_file(FILE *output) {
    char byte;
    printf("Reading file by bytes: \n");
    while (fread(&byte, sizeof(char), 1, output) == 1) {
        printf("\nReaded byte is: %d\n", byte);
        printf("High-order word is _IO_MAGIC; rest is flags: %d\n", output->_flags);
        printf("Current read pointer: %p\n", output->_IO_read_ptr);
        printf("End of get area: %p\n", output->_IO_read_end);
        printf("Start of putback+get area: %p\n", output->_IO_read_base);
        printf("Start of put area: %p\n", output->_IO_write_base);
        printf("Current put pointer: %p\n", output->_IO_write_ptr);
        printf("End of put area: %p\n", output->_IO_write_end);
        printf("Start of reserve area: %p\n", output->_IO_buf_base);
        printf("End of reserve area: %p\n", output->_IO_buf_end);

        printf("Pointer to start of non-current get area: %p\n", output->_IO_save_base);
        printf("Pointer to first valid character of backup area: %p\n", output->_IO_backup_base);
        printf("Pointer to end of non-current get area: %p\n", output->_IO_save_end);

        printf("_fileno: %d\n", output->_fileno);
        printf("_flags2: %d\n", output->_flags2);

        printf("_cur_column: %d\n", output->_cur_column);
        printf("_vtable_offset: %hhd\n", output->_vtable_offset);
        printf("_shortbuf[1]: %s\n", output->_shortbuf);
    }
    return correct;
}

status_code fseek_task_file(FILE** output) {
    if (fseek(*output, 3, SEEK_SET) != 0) {
        printf("Unable to do fseek\n");
        fclose(*output);
        return error_file;
    }
    return correct;
}

status_code fread_4bytes_task_file(FILE** output, char buff[]) {
    if (fread(buff, sizeof(char), 4, *output) != 4) {
        printf("Can't read 4 bytes\n");
        fclose(*output);
        return error_file;
    }
    return correct;
}
