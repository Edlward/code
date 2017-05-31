#include <stdio.h>
#include <vector>
#include <stdlib.h>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *resultHead = new ListNode(0);
        ListNode *result = resultHead;
        int sign = 0;

        while(1)
        {
            int val1 = l1 != NULL ? l1->val:0;
            int val2 = l2 != NULL ? l2->val:0;
            int sum = val1 + val2 + sign;

            sign = sum - 9 > 0 ? 1:0;
            result->val = sign == 1 ? sum - 10:sum;
            // printf("result:%d\t", result->val);


            if(l2 != NULL)
                l2 = l2->next;
            if(l1 != NULL)
                l1 = l1->next;
            if(l1 == NULL && l2 == NULL && sign == 0)
            {
                break;
            }
            result->next = new ListNode(0);
            result = result->next; 
        }

        // printf("return\n");
        return resultHead;
    }
};

int main()
{
    Solution solution;
    
    vector<int> vl1 = {2,4};
    vector<int> vl2 = {5,6,4};
    ListNode *l1head = new ListNode(0);
    ListNode *l2head = new ListNode(0);
    ListNode *l1 = l1head;
    ListNode *l2 = l2head;
 
    printf("vl1.size %d\n", (int)vl1.size());
    printf("vl2.size %d\n", (int)vl2.size());
    
    for(int i = 0; i < vl1.size() - 1; ++i)
    {
        l1->val = vl1[i];
        printf("%d\t", l1->val);
        l1->next = new ListNode(0);
        l1 = l1->next;
    }
    l1->val = vl1[vl1.size() - 1];
    printf("\n");

    for(int i = 0; i < vl2.size() - 1; ++i)
    {
        l2->val = vl2[i];
        printf("%d\t", l2->val);
        l2->next = new ListNode(0);
        l2 = l2->next;
    }
    l2->val = vl2[vl2.size() - 1];

    
    printf("\nsolution\n");

    ListNode * result = solution.addTwoNumbers(l1head, l2head);

    printf("l1\n");
    while(result != NULL)
    {
        printf("out:%d\n", result->val);
        result = result->next;
    }

}