#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct POINT {
    float latitude;
    float longitude;
} POINT;

// uses somewhat rare base32 mapping (https://en.wikipedia.org/wiki/geohash)
const char BASE_32_ENCODE[] = "0123456789BCDEFGHJKMNPQRSTUVWXYZ";


char *encode(POINT p, int precision) {
    // NOTE: Precision should be mulitiple of 5)
    // otherwise hash value will be incorrect (in base-32)
    unsigned int bits = 0x0;
    float xmin = -180.0, xmax = 180.0;
    float ymin = -90.0, ymax = 90.0;
    float midpoint;
    int i, char_num = 0;
    char *geohash = (char *)malloc((precision / 5) * sizeof(char *));

    for (i = 0; i < precision; i++) {

        //if even, split vertically
        if (i % 2 == 0) {
            midpoint = (xmin + xmax) / 2.0;

            if (p.longitude < midpoint) {
                xmax = midpoint;  
                bits = bits << 1;
            } else {
                xmin = midpoint;
                bits = (bits << 1) | 0x1;
            }
        } else {
            midpoint = (ymin + ymax) / 2.0;
            
            if (p.latitude < midpoint) {
                ymax = midpoint;  
                bits = bits << 1;
            } else {
                ymin = midpoint;
                bits = (bits << 1) | 0x1;
            }
        }

        if ((i + 1) % 5 == 0) {
            geohash[char_num++] = BASE_32_ENCODE[bits & 0x1F];
        }
    }

    return geohash;
}

POINT decode(char *geohash) {
    size_t size = strlen(geohash);   
    float xmin = -180.0, xmax = 180.0;
    float ymin = -90.0, ymax = 90.0;
    int i, j, bit_num = 1;
    
    for (i = 0; i < size; i++) {
        // crazy way of getting index of character in string
        uint bits = strchr(BASE_32_ENCODE, geohash[i]) - BASE_32_ENCODE;

        for (j = 4; j >= 0; j--) {
            uint bit = (bits >> j) & 0x1;

            if (bit) { 
                // longitude
                if (bit_num % 2 == 1) {
                    xmin = (xmin + xmax) / 2.0;
                } else {
                    ymin = (ymin + ymax) / 2.0;
                }
            } else {
                if (bit_num % 2 == 1) {
                    xmax = (xmin + xmax) / 2.0; 
                } else {
                    ymax = (ymin + ymax) / 2.0;  
                }
            }

            bit_num++;
        }
    }

    return (POINT) { (ymin + ymax) / 2.0, (xmin + xmax) / 2.0 };
}

int main(int argc, char *argv[]) {

    if (argc == 4) {
        POINT p = { atof(argv[1]), atof(argv[2]) };
        int precision = atoi(argv[3]);
        char *geohash = encode(p, precision);

        printf("GEOHASH ENCODE: %s\n", geohash);
    } else if (argc == 2) {
        POINT p = decode(argv[1]);

        printf("GEOHASH DECODE: (%f, %f)\n", p.latitude, p.longitude);
    } else {
        printf("Oops, no arguments\n");
    }

    return 0;
}