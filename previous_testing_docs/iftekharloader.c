 
 
// #define IMEM_SIZE 65536
// #define DMEM_SIZE 65536
// #define REGFILE_SIZE 8
// #define ARG_COUNT 2
// #define HEX_BASE 16
// #define ASCII_PRINTABLE_START 32
// #define ASCII_PRINTABLE_END 126
// #define HEADER_START 8
// #define BYTE_SIZE 2
// #define ASCII_SIZE 4
// #define IMEM_SHIFT 1
// #define DMEM_SHIFT 1
// #define BYTE_MASK 0xFF
// #define ADDRESS_SHIFT 1
// #define DATA_SHIFT 8
// #define LINE_SIZE 100
// #define READ 0x1
// #define WRITE 0x2 // RECHECK
// #define DEBUGGER_MODE 1
// #define EXECUTION_MODE 0

// void loadSRecord(const char* filename) {

//     unsigned int byte;
//     unsigned int data;

//     FILE* file = fopen(filename, "r");
//     if (!file) {
//         perror("Error opening file");
//         return;
//     }
//     char line[LINE_SIZE];
//     while (fgets(line, sizeof(line), file)) {

//         if (line[0] != 'S') continue; // Not an S-Record
//         int count, address;
//         sscanf(line + 2, "%2x%4x", &count, &address);
//         int dataLength = (count - 3) * BYTE_SIZE;

//         // Calculate checksum
//         unsigned int calculatedChecksum = calculateChecksum(line, count, dataLength);
//         unsigned int givenChecksum;
//         sscanf(line + 2 + count * BYTE_SIZE, "%2x", &givenChecksum);

//         if (calculatedChecksum != givenChecksum) {
//             printf("Checksum error in line: %s\n", line);
//             continue;
//         }

//         if (line[1] == '0') {
//             // S0 record: header
//             printf("Header (ASCII): ");
//             for (int i = 0; i < dataLength; i += BYTE_SIZE) {
//                 sscanf(line + HEADER_START + i, "%2x", &byte);
//                 printf("%c", byte);
//             }
//             printf("\n");
//             printf("Header (Bytes): ");
//             for (int i = 0; i < dataLength + BYTE_SIZE; i += BYTE_SIZE) {
//                 sscanf(line + HEADER_START + i, "%2x", &byte);
//                 printf("%02X ", byte);
//             }
//             printf("\n");
//         }
//         else if (line[1] == '1') {
//             // S1 record: IMEM data
//             for (int i = 0; i < dataLength; i += ASCII_SIZE) {
//                 sscanf(line + HEADER_START + i, "%4x", &data);
//                 IMEM[(address >> IMEM_SHIFT) + (i >> BYTE_SIZE)] = (data >> DATA_SHIFT) | ((data & BYTE_MASK) << DATA_SHIFT); // Correctly handle high and low byte
//             }
//         }
//         else if (line[1] == '2') {
//             // S2 record: DMEM data
//             for (int i = 0; i < dataLength; i += ASCII_SIZE) {
//                 sscanf(line + HEADER_START + i, "%4x", &data);
//                 DMEM[(address >> DMEM_SHIFT) + (i >> BYTE_SIZE)] = (data >> DATA_SHIFT) | ((data & BYTE_MASK) << DATA_SHIFT); // Correctly handle high and low byte
//             }
//         }
//         else if (line[1] == '9') {
//             // S9 record: starting address
//             sscanf(line + 4, "%4x", &address);
//             printf("Starting address: %04X\n\n", address);
//             reg_file[7] = address; // Store starting address in PC
//         }
//     }
//     fclose(file);
// }