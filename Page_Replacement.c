#include <stdio.h>
#include <stdlib.h>

#define MAX_PAGES 100

// Function to implement FIFO Page Replacement
int FIFO(int pages[], int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int));
    int front = 0, page_faults = 0;

    // Initialize frame
    for (int i = 0; i < frames; i++) frame[i] = -1;

    printf("\nFIFO Page Replacement:\n");

    for (int i = 0; i < n; i++) {
        int found = 0;

        // Check if page is already in frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        // Page fault handling
        if (!found) {
            frame[front] = pages[i];
            front = (front + 1) % frames;
            page_faults++;
        }

        // Display current frame state
        printf("Step %d: ", i + 1);
        for (int j = 0; j < frames; j++) {
            if (frame[j] == -1) printf("_ ");
            else printf("%d ", frame[j]);
        }
        printf("\n");
    }

    free(frame);
    return page_faults;
}

// Function to implement LRU Page Replacement
int LRU(int pages[], int n, int frames) {
    int *frame = (int *)malloc(frames * sizeof(int));
    int *recent = (int *)malloc(frames * sizeof(int));
    int page_faults = 0;

    // Initialize frame and tracking
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        recent[i] = -1;
    }

    printf("\nLRU Page Replacement:\n");

    for (int i = 0; i < n; i++) {
        int found = 0;

        // Check if page is already in frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                recent[j] = i;
                break;
            }
        }

        // Page fault handling
        if (!found) {
            int lru_idx = 0;

            // Find least recently used page
            for (int j = 1; j < frames; j++) {
                if (recent[j] < recent[lru_idx]) {
                    lru_idx = j;
                }
            }

            frame[lru_idx] = pages[i];
            recent[lru_idx] = i;
            page_faults++;
        }

        // Display current frame state
        printf("Step %d: ", i + 1);
        for (int j = 0; j < frames; j++) {
            if (frame[j] == -1) printf("_ ");
            else printf("%d ", frame[j]);
        }
        printf("\n");
    }

    free(frame);
    free(recent);
    return page_faults;
}

int main() {
    int n, frames, pages[MAX_PAGES];

    // Input validation
    printf("Enter the number of pages (minimum 10): ");
    scanf("%d", &n);
    if (n < 10) {
        printf("Error: At least 10 pages are required.\n");
        return EXIT_FAILURE;
    }

    printf("Enter the page reference string (space-separated): ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of frames (minimum 3): ");
    scanf("%d", &frames);
    if (frames < 3) {
        printf("Error: At least 3 frames are required.\n");
        return EXIT_FAILURE;
    }

    // Run algorithms
    int fifo_faults = FIFO(pages, n, frames);
    int lru_faults = LRU(pages, n, frames);

    // Display results
    printf("\nPage Replacement Results:\n");
    printf("FIFO Page Faults: %d\n", fifo_faults);
    printf("LRU Page Faults: %d\n", lru_faults);

    return EXIT_SUCCESS;
}
