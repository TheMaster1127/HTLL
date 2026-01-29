#include <stdio.h>

int main() {
    int i;
    for (i = 1; i <= 10000; i++) {
        FILE *fp = fopen("testTXT1.txt", "a");  // Open in append mode
        if (!fp) {
            perror("Failed to open file");
            return 1;
        }

        fprintf(fp, "%d\n", i);  // Write number + newline
        fclose(fp);              // Close immediately

        if (i == 69) {
            printf("%d\n", i);   // Print when i == 69
        }
    }

    printf("%d\n", i - 1);  // Print final i
    return 0;
}
