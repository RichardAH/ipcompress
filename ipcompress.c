#include <stdio.h>
#include <stdlib.h>

struct tree {
    struct tree* parent;
    struct tree* children[2];
    int sub_nodes;
};

struct tree* new_node(struct tree* parent) {
    struct tree* root = (struct tree*)malloc(sizeof(struct tree));
    root->sub_nodes = 0;
    root->children[0] =NULL;
    root->children[1] =NULL;
    root->parent = parent;
    return root;
}

void print_tree(struct tree* upto, int level, double threshold) {

    if (upto == NULL) return;


    // make subnet
    unsigned long ip = 0;
    int steps = 0;
//    printf("\n");
    for (struct tree* n = upto->parent, *p = upto; n != NULL; n = n->parent) {
        unsigned long toadd =  n->children[1] == p ? ( 1UL << steps ) : 0;
        ip += toadd ;
    //    printf("level: %d, steps: %d, bit: %d, calc: %d, ip: %x\n", level, steps,  n->children[1] == p, toadd, ip);
        p = n;
        steps++;
    }

    ip <<= (32 - steps); 


    double calc = ( ( upto->sub_nodes ) * 1.0f )/ (  (1UL<<(32-level))  * 1.0f ) * 100.0f;

    for (int bit = 0; bit < 2; ++bit) {
        if (upto->children[bit]) {
            ip += bit;
             if (level == 31) {
                int a = (ip >> 24) & 0xff;
                int b = (ip >> 16) & 0xff;
                int c = (ip >> 8) & 0xff;
                int d = ip & 0xff;
                printf("%d.%d.%d.%d\n", a,b,c,d);
                continue;
            }

            if (calc >= threshold && level <= 24) {
                //for (int i = 0; i < level; ++i) putc(' ', stdout);
                //printf("%c - count: %d/%lu [%f] - %x - %d\n", (bit ? '1' : '0'), upto->sub_nodes, (1UL<<(31-level)), calc, ip, level + 1);
                int a = (ip >> 24) & 0xff;
                int b = (ip >> 16) & 0xff;
                int c = (ip >> 8) & 0xff;
                int d = ip & 0xff;
                printf("%d.%d.%d.%d/%d\n", a,b,c,d, level, calc);
            } else {
                print_tree(upto->children[bit], level+1, threshold);
            }

        } 
    }

    return;
}

int main(int argc, char** argv) {

    if (argc != 3)
        return fprintf(stderr, "useage: %s <file-containgin-one-ip-per-row> <percent-threshold-integer>\n", argv[0]);


    int threshold = 0;
    if (!sscanf(argv[2], "%d", &threshold))
        return fprintf(stderr, "could not parse threshold `%s`\n", argv[2]);

    FILE* f = fopen(argv[1], "r");
    if (f == NULL)
        return fprintf(stderr, "could not open %s for reading\n", argv[1]);


    struct tree* root = new_node(NULL);

    char ip_buffer[32];

    while ( fgets( ip_buffer, 31, f) ) {

        int a,b,c,d;
        if (sscanf(ip_buffer, "%d.%d.%d.%d",&a,&b,&c,&d) !=  4) continue;

        unsigned long ip = (a  <<  24) + (b << 16) + (c << 8) + d;

        struct tree* upto = root;

        for (int i = 31; i >= 0; --i) {
            int index = 31 - i;
            int bit = (ip >> i) & 1;

            if (upto->children[bit] == NULL)
                upto->children[bit] = new_node(upto);
            
            upto->sub_nodes++;

            upto = upto->children[bit];            
        }

    }

    print_tree(root, 0, (double)threshold);

    fclose(f);

    return 0;
}
