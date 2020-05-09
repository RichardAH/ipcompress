#include <stdio.h>
#include <stdlib.h>

struct tree {
    struct tree* children[2];
    int sub_nodes;
};

struct tree* new_node() {
    printf("%d\n", sizeof(struct tree));
    struct tree* root = (struct tree*)malloc(sizeof(struct tree));
    root->sub_nodes = 0;
    root->children[0] =NULL;
    root->children[1] =NULL;
    return root;
}

void print_tree(struct tree* upto, int level) {


    if (upto->children[0]) {
        printf("%.*s0 - count %d : %d\n", level, "                                 ", upto->sub_nodes, level  );
        print_tree(upto->children[0], level+1);
    } 

    if (upto->children[1]) {
        printf("%.*s1 - count %d : %d\n", level, "                                 ", upto->sub_nodes, level  );
        print_tree(upto->children[0], level+1);
    } 
}

int main(int argc, char** argv) {

    if (argc != 2)
        return fprintf(stderr, "useage: %s file-one-ip-per-row\n", argv[0]);

    FILE* f = fopen(argv[1], "r");
    if (f == NULL)
        return fprintf(stderr, "could not open %s for reading\n", argv[1]);


    struct tree* root = new_node();

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
                upto->children[bit] = new_node(bit);
            
            upto->sub_nodes++;

            upto = upto->children[bit];            
        }

    }

    print_tree(root, 0);

    fclose(f);

    return 0;
}
