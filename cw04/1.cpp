/*
 * Implementacja SkipList
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

struct SLNode {
    int key;
    SLNode **next;
};

struct SkipList {
    SLNode *first;
    SLNode *last;
    int maxLevel;
};

SLNode *find(SkipList *SL, int key) {
    SLNode *p = SL->first;
    for (int lvl = SL->maxLevel - 1; lvl >= 0; lvl--)
        while (key > p->next[lvl]->key) p = p->next[lvl];

    p = p->next[0];
    if (p->key == key) return p;
    return nullptr;
}

SkipList *init(int maxLevel) {
    SLNode *last = new SLNode { INT_MAX, nullptr };
    SLNode **next = new SLNode*[maxLevel];
    for (int i = 0; i < maxLevel; i++) next[i] = last;
    SLNode *first = new SLNode { INT_MIN, next };
    return new SkipList { first, last, maxLevel };
}

int getLevel() {
    int lvl = 1;
    while (((double) rand() / RAND_MAX) < 0.5) lvl++;
    return lvl;
}

void insert(SkipList *SL, int key) {
    if (find(SL, key) != nullptr) return;
    int newLevel = getLevel();
    if (newLevel > SL->maxLevel) newLevel = SL->maxLevel;
    SLNode *newNode = new SLNode { key, new SLNode*[newLevel] };

    SLNode **visited = new SLNode*[newLevel];
    
    SLNode *p = SL->first;
    for (int lvl = SL->maxLevel - 1; lvl >= 0; lvl--) {
        while (key > p->next[lvl]->key) p = p->next[lvl];
        if (lvl < newLevel) visited[lvl] = p;
    }

    for (int lvl = newLevel - 1; lvl >= 0; lvl--) {
        newNode->next[lvl] = visited[lvl]->next[lvl];
        visited[lvl]->next[lvl] = newNode;
    }
}


int main() {
    srand(time(NULL));

    SkipList *SL = init(5);
    insert(SL, 10);
    insert(SL, 1);
    insert(SL, 4);
    insert(SL, 2);
    insert(SL, 3);
    std::cout << find(SL, 1)->key;
    std::cout << getLevel() << " " << getLevel();

    for (int lvl = SL->maxLevel - 1; lvl >= 0; lvl--) {
        SLNode *p = SL->first;
        while (p->next != nullptr) {
            std::cout << p->key << " ";
            p = p->next[lvl];
        }
        std::cout << std::endl;
    }

    std::cout << find(SL, -1);
    std::cout << find(SL, 12);
    std::cout << find(SL, 10);
}
