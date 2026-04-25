
#include <cstdio>
#include <iostream>

using namespace std;

const int N = 500005;

struct Node {
    int val;
    unsigned int priority;
    int l, r;
} tree[N];

int root = 0, cnt = 0;
unsigned int seed = 12345;

unsigned int my_rand() {
    seed = seed * 1103515245 + 12345;
    return seed;
}

void split(int p, int val, int &l, int &r) {
    if (!p) {
        l = r = 0;
        return;
    }
    if (tree[p].val <= val) {
        l = p;
        split(tree[p].r, val, tree[l].r, r);
    } else {
        r = p;
        split(tree[p].l, val, l, tree[r].l);
    }
}

int merge(int l, int r) {
    if (!l || !r) return l + r;
    if (tree[l].priority > tree[r].priority) {
        tree[l].r = merge(tree[l].r, r);
        return l;
    } else {
        tree[r].l = merge(l, tree[r].l);
        return r;
    }
}

void insert(int val) {
    int l, r;
    split(root, val, l, r);
    cnt++;
    tree[cnt].val = val;
    tree[cnt].priority = my_rand();
    tree[cnt].l = tree[cnt].r = 0;
    root = merge(merge(l, cnt), r);
}

void erase(int val) {
    int l, r, mid;
    split(root, val, l, r);
    split(l, val - 1, l, mid);
    root = merge(l, r);
}

int find_next(int val) {
    int res = -1;
    int curr = root;
    while (curr) {
        if (tree[curr].val > val) {
            res = tree[curr].val;
            curr = tree[curr].l;
        } else {
            curr = tree[curr].r;
        }
    }
    return res;
}

int find_prev(int val) {
    int res = -1;
    int curr = root;
    while (curr) {
        if (tree[curr].val < val) {
            res = tree[curr].val;
            curr = tree[curr].r;
        } else {
            curr = tree[curr].l;
        }
    }
    return res;
}

enum Direction { UP, DOWN };

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int current_floor = 0;
    Direction dir = UP;

    char op[20];
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        if (op[0] == 'l') { // locate
            printf("%d\n", current_floor);
        } else if (op[0] == 'a' && op[1] == 'd') { // add
            int x;
            scanf("%d", &x);
            insert(x);
        } else if (op[0] == 'c') { // cancel
            int x;
            scanf("%d", &x);
            erase(x);
        } else if (op[0] == 'a' && op[1] == 'c') { // action
            if (root == 0) continue;
            if (dir == UP) {
                int next_f = find_next(current_floor);
                if (next_f != -1) {
                    current_floor = next_f;
                    erase(current_floor);
                } else {
                    dir = DOWN;
                    int prev_f = find_prev(current_floor);
                    if (prev_f != -1) {
                        current_floor = prev_f;
                        erase(current_floor);
                    }
                }
            } else { // dir == DOWN
                int prev_f = find_prev(current_floor);
                if (prev_f != -1) {
                    current_floor = prev_f;
                    erase(current_floor);
                } else {
                    dir = UP;
                    int next_f = find_next(current_floor);
                    if (next_f != -1) {
                        current_floor = next_f;
                        erase(current_floor);
                    }
                }
            }
        }
    }

    return 0;
}
