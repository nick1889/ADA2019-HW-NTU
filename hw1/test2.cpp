//
// Created by Nick on 2019-10-13.
// reference: https://blog.csdn.net/Gloria971111/article/details/79173806
//
#include <iostream>
#include <vector>

using namespace std;

class ResultNode{
public:
    long result;
    long max_prefix_sum;
    long max_suffix_sum;
    long interval_sum;
    ResultNode *left_result;
    ResultNode *right_result;
};

ResultNode* max_sub(vector<int> &A, int low, int high)
{
    if(low == high)
    {
        ResultNode *node = new ResultNode();
        node->max_prefix_sum = A[low];
        node->max_suffix_sum = A[low];
        node->interval_sum = A[low];
        node->result = A[low];
        return node;
    }
    int mid = (low + high - 1)/2;
    ResultNode *left_result = max_sub(A, low, mid);
    ResultNode *right_result = max_sub(A, mid + 1, high);
    ResultNode *node = new ResultNode();
    node->left_result = left_result;
    node->right_result = right_result;
    node->interval_sum = left_result->interval_sum + right_result->interval_sum;
    node->max_prefix_sum = max(left_result->max_prefix_sum,left_result->interval_sum+right_result->max_prefix_sum);
    node->max_suffix_sum = max(right_result->max_suffix_sum,left_result->max_suffix_sum+right_result->interval_sum);
    node->result = max(max(left_result->result,right_result->result),left_result->max_suffix_sum+right_result->max_prefix_sum);
    return node;
}

ResultNode* max_sub2(ResultNode &root, vector<int> &A, int low, int high, int pi)
{
    //cout << low << " , " << high << endl;
    if(low == high)
    {
        if(low == pi)
        {
            ResultNode *sub = max_sub(A, low, high);
            return sub;
        }
        else
            return &root;
    }

    int mid = (low + high - 1)/2;
//    cout << low << " " << high << endl;
    ResultNode *left_result;
    ResultNode *right_result;
    if(pi <= mid)
    {
        right_result = root.right_result;
//        cout << "get right result "<< right_result.result << endl;
        left_result = max_sub2(*root.left_result,A,low,mid,pi);
        root.left_result = left_result;
    }
    else
    {
        left_result = root.left_result;
//        cout << "get left result " << left_result.result <<endl;
        right_result = max_sub2(*root.right_result,A,mid+1,high,pi);
        root.right_result = right_result;
    }
    if(low <= pi <= high)
    {
        root.interval_sum = left_result->interval_sum + right_result->interval_sum;
        root.max_prefix_sum = max(left_result->max_prefix_sum,left_result->interval_sum+right_result->max_prefix_sum);
        root.max_suffix_sum = max(right_result->max_suffix_sum,left_result->max_suffix_sum+right_result->interval_sum);
        root.result = max(max(left_result->result,right_result->result),left_result->max_suffix_sum+right_result->max_prefix_sum);
//      cout << "comput cross result " << cross_result << " " << endl;
    }
    return &root;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N = 0, Q = 0;
    cin >> N >> Q;
    vector<int> A(N);
    vector<int> p(Q),v(Q);
    for(int i = 0; i < N; i++)
    {
        cin >> A[i];
    }
    for(int i = 0; i < Q; i++)
    {
        cin >> p[i] >> v[i];
    }
    ResultNode *root = max_sub(A,0,N-1);
    if(root->result <= 0)
        cout << 0 << endl;
    else
        cout << root->result << endl;

    for(int i = 0; i < Q; i++)
    {
        long res = 0;
        A[p[i] - 1] = v[i];
        root = max_sub2(*root,A,0,N-1,p[i]-1);
        res = root->result;
        if(res <= 0)
            cout << 0 << endl;
        else
            cout << res <<endl;
    }
    return 0;
}