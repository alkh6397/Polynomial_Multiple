#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_DEGREE 101 // 다항식의 최대차수 + 1

typedef struct {
    float x_coef[MAX_DEGREE];
    int x_degree[MAX_DEGREE];

    float y_coef[MAX_DEGREE];
    int y_degree[MAX_DEGREE];

    float z_coef[MAX_DEGREE];
    int z_degree[MAX_DEGREE];
} polynomial;

void print_polynomial(polynomial *p, int term){
    printf("============================\n");
    for(int i=0;i<=term;i++){
        printf("term: %d\n\n", i);
        printf("x_coef: %f\n", p->x_coef[i]);
        printf("x_degree: %d\n", p->x_degree[i]);
        printf("y_coef: %f\n", p->y_coef[i]);
        printf("y_degree: %d\n", p->y_degree[i]);
        printf("z_coef: %f\n", p->z_coef[i]);
        printf("z_degree: %d\n", p->z_degree[i]);
        printf("==============================\n");
    }
}

void polynomial_reset(polynomial *p){
    for(int i=0;i<MAX_DEGREE;i++){
        p->x_coef[i] = 1;
        p->x_degree[i] = 0;
        p->y_coef[i] = 1;
        p->y_degree[i] = 0;
        p->z_coef[i] = 1;
        p->z_degree[i] = 0;
    }
}

void test_string_print(char *src){
    for(int i=0;i<50;i++){
        printf("%c", src[i]);
    }
    printf("\n\n");
}

void normalize(char *src, char *result) {
    int src_index = 0;
    int result_index = 0;
    int sign = 0; // 0:'+', 1:'-'
    int sign_scan_index = 0;
    while(1){
        // 공백 제거
        if(src[src_index] == ' '){
            //공백 있을 때는 패스
            //test_string_print(result);
        }
        else if(src[src_index] == '\0'){
            result[result_index] = '\0';
            //test_string_print(result);
            break;
        }
        else{
            if(src[src_index] == '+' || src[src_index] == '-'){
                // 부호를 한번 읽기 시작하면 우선 부호 덩어리를 쭉 읽어서 +인지 -인지 결정하고 부호를 하나만 요약해서 result에 넣음
                //printf("Start sign scan\n\n");
                //test_string_print(result);
                while(1){
                    if(src[src_index + sign_scan_index] == '+'){
                        // +이면 sign을 가만히 놔둠
                        //printf("+scan\n\n");
                        //test_string_print(result);
                    }
                    else if(src[src_index + sign_scan_index] == '-'){
                        // -이면 sign을 반전함
                        //printf("-scan\n\n");
                        //test_string_print(result);
                        if(sign == 0) sign = 1;
                        else if(sign == 1) sign  = 0;
                    }
                    else if((src[src_index + sign_scan_index] >= '0' && src[src_index + sign_scan_index]<='9') ||
                                (src[src_index + sign_scan_index] == 'x') ||
                                (src[src_index + sign_scan_index] == 'y') ||
                                (src[src_index + sign_scan_index] == 'z')){
                        // 부호 다음에 나올만한 것들: 숫자, x, y, z 문자
                        // 이것들을 읽었다는건 부호를 다 읽었다는 뜻이므로 while 탈출
                        //printf("sign scan finish\n\n");
                        //test_string_print(result);
                        if(sign == 0){
                            result[result_index] = '+';
                            //printf("+sign insert to result[%d]\n\n", result_index);
                            //test_string_print(result);
                        }
                        else if(sign == 1){
                            result[result_index] = '-';
                            //printf("-sign insert to result[%d]\n\n", result_index);
                            //test_string_print(result);
                        }
                        // 부호가 지나온 칸만큼 src_index를 이동해서 그다음 숫자 or 문자부터 읽게 함
                        src_index +=sign_scan_index;
                        //printf("sign scan index: %d\n\n", sign_scan_index);
                        result_index++;
                        //test_string_print(result);
                        break;
                    }
                    sign_scan_index++;
                }
                sign_scan_index = 0;
                sign = 0;
            }
            result[result_index] = src[src_index];
            result_index++;
        }
        src_index++;
    }
    
    if(result[0] == '-' || result[0] == '+'){
        // 맨 앞 부호가 +, -이면 놔두지만
    }
    else{
        // 맨 앞 부호가 존재하지 않는다면, 그것은 +부호가 생략된 것을 말함.
        // 파싱의 편의성을 위해 강제로 +를 이식하겠음
        int length = 0;
        for(int i=0;result[i] != '\0';i++){
            length++;
        }
        result[length+1] = '\0';
        for(int i=length;i>=1;i--){
            result[i] = result[i-1];
        }
        result[0] = '+';
    }
    
}

int main(){
    polynomial a;
    polynomial b;
    char a_str[100];
    char b_str[100];
    char a_str_norm[100];
    char b_str_norm[100];
    printf("A: ");
    gets(a_str);
    //printf("B: ");
    //gets(b_str);
    
    // 다항식 리셋
    polynomial_reset(&a);
    
    printf("Before normalizing:\n%s\n", a_str);
    //printf("Before normalizing:\n%s\n%s\n", a_str, b_str);

    normalize(a_str, a_str_norm);
    //normalize(b_str, b_str_norm);

    printf("Normalized:\n%s\n", a_str_norm);
    //printf("Normalized:\n%s\n%s\n", a_str_norm, b_str_norm);
    return 0;
    
}
