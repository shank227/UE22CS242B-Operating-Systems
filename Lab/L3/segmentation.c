#include <stdio.h> 
#include <stdlib.h> 

struct list { 
    int seg_num; 
    int base_addr; 
    int seg_limit; 
    struct list *next; 
} *p; 

void insert(struct list *q, int base_addr, int seg_limit, int seg_num) { 
    if (p == NULL) { 
        p = malloc(sizeof(struct list)); 
        p->seg_limit = seg_limit; 
        p->base_addr = base_addr; 
        p->seg_num = seg_num; 
        p->next = NULL; } 
    else { 
        while (q->next != NULL) { 
            q = q->next; 
            printf("yes\n"); 
        } 
        q->next = malloc(sizeof(struct list)); 
        q->next->seg_limit = seg_limit; 
        q->next->base_addr = base_addr; 
        q->next->seg_num = seg_num; 
        q->next->next = NULL; 
    } 
}

int find(struct list *q, int seg_num) { 
    while (q->seg_num != seg_num) { 
        q = q->next; 
    } 
    return q->seg_limit; 
} 

int search(struct list *q, int seg_num) { 
    while (q->seg_num != seg_num) { 
        q = q->next; 
    } 
    return q->base_addr; 
} 

int main() { 
    p = NULL; 
    int seg, offset, limit, base, c, s, physical; 
    printf("Enter segment table : \n"); 
    printf("Enter -1 (segment value) for termination : \n"); 
    do { 
        printf("Enter segment number : "); 
        scanf("%d", &seg); 
        if (seg != -1) { 
            printf("Enter base value : "); 
            scanf("%d", &base); 
            printf("Enter value for limit : "); 
            scanf("%d", &limit); 
            insert(p, base, limit, seg); 
        } 
        } while (seg != -1); 

        printf("Enter offset : "); 
        scanf("%d", &offset); 
        printf("Enter segmentation number : "); 
        scanf("%d", &seg); c = find(p, seg); 
        s = search(p, seg); 
        
        if (offset < c) { 
            physical = s + offset; 
            printf("Address in physical memory : %d\n", physical); 
        } 
        else { 
            printf("error\n"); 
        }
}
