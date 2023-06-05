#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to reverse the order of bytes in a buffer
void reverse_bytes(char *buffer, size_t size) {
    char temp;
    size_t start = 0;
    size_t end = size - 1;

    // Swap the bytes from the start and end until the middle of the buffer is reached
    while (start < end) {
        // Swap the bytes at the current start and end positions
        temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;

        // Move the start position forward and the end position backward
        start++;
        end--;
    }
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    // Open the file for reading in binary mode
    FILE *file = fopen(filename, "rb");

    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Move the file position to the end to determine the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);

    // Move the file position back to the beginning
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer to store the file content
    char *buffer = malloc(file_size);

    // Check if memory allocation was successful
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    // Read the content of the file into the buffer
    if (fread(buffer, 1, file_size, file) != file_size) {
        perror("Error reading file");
        fclose(file);
        free(buffer);
        return 1;
    }

    // Close the file
    fclose(file);

    // Reverse the order of bytes in the buffer
    reverse_bytes(buffer, file_size);

    // Open the file for writing in binary mode
    file = fopen(filename, "wb");

    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening file");
        free(buffer);
        return 1;
    }

    // Write the reversed buffer back to the file
    if (fwrite(buffer, 1, file_size, file) != file_size) {
        perror("Error writing to file");
        fclose(file);
        free(buffer);
        return 1;
    }

    // Close the file
    fclose(file);

    // Free the memory allocated for the buffer
    free(buffer);

    return 0;
}
