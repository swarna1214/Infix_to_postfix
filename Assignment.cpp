#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
string post[1000];         //This is work as the stack( later we will use it as post stack(postfix stack))
map<char,ll>precedence;
void evaluate(ll k)
{
    vector<string>v(post,post+k);   // Copying the whole stack in the vector v to evaluate the expression
    ll i;
    for(i=2; i<v.size(); i++)       // starting with i=2 because it is guaranteed there are at least two numbers before encountering the first operator
    {
        if(v[i].size()==1&&precedence[v[i][0]]>0)// if the size of the term is 1 then it might be operator. if it is an operator than it has a non-zero precedence value
        {
            long double a=stod(v[i-1]),b=stod(v[i-2]),ans=0;   // converting two  string to two numeric value before the operator we encountered
            switch(v[i][0])                                    // passing the operator to know what operation we have to do on these two values
            {
                  case '+':
                    ans=b+a;
                    break;
                  case '-':
                    ans=b-a;
                    break;
                  case '*':
                    ans=b*a;
                    break;
                  case '/':
                    ans=b/a;
                    break;
                  case  '%':
                     ans=fmod(b,a);
                     break;
                  case '^':
                    ans=pow(b,a);
                    break;
            }
           v.erase(v.begin()+i);           //erasing the operator
           v[i-2]=to_string(ans);          // keeping the result as string 
           v.erase(v.begin()+i-1);         //erasing the number before the operator
           i-=2;                           //As the size of the vector will decrease by two after erasing the operator and the number
        }
    }
    cout<<"\nResult: "<<v[i-1];
}
int main()
{
    string input[1000];                     //As there might be blank space between expressions, we are taking the input as an array of strings.
    ll i,j,k=0,c=0;                         //  As a result we will be able to take the whole infix expression and evaluate as well.
    for(i=0;; i++)
    {
        cin>>input[i];
        c++;           // Here we are counting the number of strings in the string array so that we can later track all parts of each string element
        if(getchar()=='\n')          
            break;
    }
    vector<char>operators;    // We are making a operator stack to work with operators and it will later help us to convert expression and evaluate.
    precedence['(']=4;        // Here we are setting the precedence of operators with some numeric value so that it gets easier to consider
    precedence[')']=4;        // their priority upon each other
    precedence['^']=3;
    precedence['*']=2;
    precedence['/']=2;
    precedence['%']=2;
    precedence['+']=1;
    precedence['-']=1;

    for(i=0; i<c; i++)
    {
        for(j=0; input[i][j]!='\0'; j++)              //Working with each part of the string array
        {
            if(precedence[input[i][j]]==0)            // if precedence is zero for some character of a string it means its not an operator(because we have already set
            {                                                                       // precedence(non-zero value) for each operator, hence its a numeric value
                ll index=j,len=0;
                while(precedence[input[i][j]]==0&&input[i][j]!='\0')       // As we just get a numeric value(digit of a number), we have to get the whole number
                {                                                          // That's why we are running this loop until we encounter another operator (untill the end of the string)
                    len++;   //The length of the whole number
                    j++;     // After passing the digit, we won't need this position again. so we are just incrementing j's value.
                }
                post[k]=input[i].substr(index,len); // we are stacking the whole number as k-th string. Here we are also building the postfix expression
                k++;
                j--;                     // Consider the expression 23+2, as soon as we encounter '+', the while loop will stop, but the next tracking will start from
            }                         //'2'. We are not expecting this, so in order to track the '+' too, we are decrementing j. thus we will start from '+' next time
            else
            {
                if(operators.size()==0||input[i][j]=='(')  // If the operator stack is empty or the operator we encountered is '(' we gonna put it into operator Stack
                {
                    operators.push_back(input[i][j]);
                }
                else if(input[i][j]==')')
                {
                    while(operators[operators.size()-1]!='('&&operators.size()>=1)  // We will push operators(except '(')  into the post stack until we meet the last '(' that we put
                    {                                               // previously in the operator stack
                        post[k].push_back(operators[operators.size()-1]);
                        k++;
                        operators.pop_back();                      // we are eliminating the operator (what are between the parentheses) from operator stack
                    }
                    operators.pop_back();                          // We are also eliminating '('
                }
                else
                {
                    while(precedence[input[i][j]]<=precedence[operators[operators.size()-1]]&&operators.size()>=1&&operators[operators.size()-1]!='(')
                    {                                                                                                                 //The conditions in while loop:
                        post[k].push_back(operators[operators.size()-1]);         // 1.In order to stack the operator with higher precedence in post stack we have to
                        k++;                                                    //check the precedence of the current operator is lower or equal to the last operator
                        operators.pop_back();                                                                         // that we put in the operator stack previously
                    }                                                                                                   // 2. in order to get rid of overflow problem
                    operators.push_back(input[i][j]);                     //3.  consider the case 3*(2*3-1), as soon as we will encounter '-', we will stack * of 2*3
                }                             //but we can't go further and stack '*' of 3* too. Cause this '*' is out of the parentheses that we are working between
            }
        }
    }
    if(operators.size()>=1)       // if there are still some operators left in the operator stack, we will stack them in the post stack
    {
        while(operators.size()!=0)
        {
            post[k].push_back(operators[operators.size()-1]);
            k++; // k is the number of terms in the postfix expression e.g 2-3+1, 5 terms
            operators.pop_back();
        }
    }
    cout<<"Postfix expression:\n";
    for(i=0; i<k; i++) //Showing the postfix expression that we just got
    {
        cout<<post[i]<<" ";
    }
    evaluate(k);
}
