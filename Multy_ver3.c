#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_DEGREE 101 // 다항식의 최대차수 + 1

typedef struct {
    int x_coef[MAX_DEGREE];
    int x_degree[MAX_DEGREE];

    int y_coef[MAX_DEGREE];
    int y_degree[MAX_DEGREE];

    int z_coef[MAX_DEGREE];
    int z_degree[MAX_DEGREE];
} polynomial;

void print_polynomial(polynomial *p, int term){
    printf("============================\n");
    for(int i=0;i<=term;i++){
        printf("term: %d\n\n", i);
        printf("x_coef: %d\n", p->x_coef[i]);
        printf("x_degree: %d\n", p->x_degree[i]);
        printf("y_coef: %d\n", p->y_coef[i]);
        printf("y_degree: %d\n", p->y_degree[i]);
        printf("z_coef: %d\n", p->z_coef[i]);
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

int parser(polynomial *p, char *str){
    int index = 0;
    int sign = 0;
    int coef_buffer = 0;
    int degree_buffer = 0;
    int term = -1;
    int xyz_mode = -1; // 차수가 x, y, z중 뭐에 대한 차수인지 알려줌, 0:x, 1:y, 2:z
    while(1){
        if(str[index] == '\0'){
            if(xyz_mode != -1){
                switch (xyz_mode)
                {
                case 0:
                    p->x_degree[term] = degree_buffer;
                    break;
                case 1:
                    p->y_degree[term] = degree_buffer;
                    break;
                case 2:
                    p->z_degree[term] = degree_buffer;
                    break;
                }
                xyz_mode = -1;
                degree_buffer = 0;
            }
            else if(xyz_mode == -1){
                // 상수항 처리
                if(sign == 1) coef_buffer*=-1;
                p->x_coef[term] = coef_buffer;
            }
            break;
        }
        else if(str[index] == '+'){
            term++;
            sign = 0;
            if(xyz_mode != -1){
                switch (xyz_mode)
                {
                case 0:
                    p->x_degree[term-1] = degree_buffer;
                    break;
                case 1:
                    p->y_degree[term-1] = degree_buffer;
                    break;
                case 2:
                    p->z_degree[term-1] = degree_buffer;
                    break;
                }
                xyz_mode = -1;
                degree_buffer = 0;
            }
        }
        else if(str[index] == '-'){
            term++;
            sign = 1;
            if(xyz_mode != -1){
                switch (xyz_mode)
                {
                case 0:
                    p->x_degree[term-1] = degree_buffer;
                    break;
                case 1:
                    p->y_degree[term-1] = degree_buffer;
                    break;
                case 2:
                    p->z_degree[term-1] = degree_buffer;
                    break;
                }
                xyz_mode = -1;
                degree_buffer = 0;
            }
        }
        else if(str[index] >= '0' && str[index] <= '9'){
            if(xyz_mode == -1){
                // 계수 쌓기
                coef_buffer*=10;
                coef_buffer+=str[index]-'0';
            }
            else{
                // 차수 쌓기
                degree_buffer*=10;
                degree_buffer+=str[index] - '0';
            }
            
        }
        else if(str[index] == 'x' || str[index] == 'y' || str[index] == 'z'){
            if(coef_buffer == 0) coef_buffer = 1;
            if(sign == 1) coef_buffer*=-1;
            switch (str[index])
            {
            case 'x':
                p->x_coef[term] = coef_buffer;
                xyz_mode = 0;
                break;
            case 'y':
                p->y_coef[term] = coef_buffer;
                xyz_mode = 1;
                break;
            case 'z':
                p->z_coef[term] = coef_buffer;
                xyz_mode = 2;
                break;
            }
            coef_buffer = 0;
            sign = 0;
        }
        else if(str[index] == '*'){
            if(xyz_mode != -1){
                switch (xyz_mode)
                {
                case 0:
                    p->x_degree[term] = degree_buffer;
                    break;
                case 1:
                    p->y_degree[term] = degree_buffer;
                    break;
                case 2:
                    p->z_degree[term] = degree_buffer;
                    break;
                }
                xyz_mode = -1;
                degree_buffer = 0;
            }
        }
        index++;
    }
    return term;
}

void sort(polynomial *c, int c_term){
    int max = c->y_degree[0];
    int max_index = 0;
    int temp_x_coef = 0;
    int temp_x_degree = 0;
    int temp_y_coef = 0;
    int temp_y_degree = 0;
    int temp_z_coef = 0;
    int temp_z_degree = 0;
    for(int i=0;i<=c_term;i++){
        max = c->y_degree[i];
        max_index = i;
        for(int j=i;j<=c_term;j++){
            if(c->y_degree[j]>max){
                max = c->y_degree[j];
                max_index = j;
            }
        }
        temp_x_coef = c->x_coef[i];
        temp_x_degree = c->x_degree[i];
        temp_y_coef = c->y_coef[i];
        temp_y_degree = c->y_degree[i];
        temp_z_coef = c->z_coef[i];
        temp_z_degree = c->z_degree[i];
        c->x_coef[i] = c->x_coef[max_index];
        c->x_degree[i] = c->x_degree[max_index];
        c->y_coef[i] = c->y_coef[max_index];
        c->y_degree[i] = c->y_degree[max_index];
        c->z_coef[i] = c->z_coef[max_index];
        c->z_degree[i] = c->z_degree[max_index];
        c->x_coef[max_index] = temp_x_coef;
        c->x_degree[max_index] = temp_x_degree;
        c->y_coef[max_index] = temp_y_coef;
        c->y_degree[max_index] = temp_y_degree;
        c->z_coef[max_index] = temp_z_coef;
        c->z_degree[max_index] = temp_z_degree;
    }
}

void multiple(polynomial *a, polynomial *b, polynomial *c, int a_term, int b_term, int *c_term){
    for(int i=0;i<=a_term;i++){
        for(int j=0;j<=b_term;j++){
            c->x_coef[*c_term] = a->x_coef[i] * b->x_coef[j];
            c->x_degree[*c_term] = a->x_degree[i] + b->x_degree[j];
            c->y_coef[*c_term] = a->y_coef[i] * b->y_coef[j];
            c->y_degree[*c_term] = a->y_degree[i] + b->y_degree[j];
            c->z_coef[*c_term] = a->z_coef[i] * b->z_coef[j];
            c->z_degree[*c_term] = a->z_degree[i] + b->z_degree[j];
            
            // 변수가 하나인 경우 계수 붙이기 처리
            if(c->x_degree[*c_term] == 0 && c->y_degree[*c_term] == 0){
                c->z_coef[*c_term] = c->x_coef[*c_term] * c->y_coef[*c_term] * c->z_coef[*c_term];
                c->x_coef[*c_term] = 1;
                c->y_coef[*c_term] = 1;
            }
            else if(c->x_degree[*c_term] == 0 && c->z_degree[*c_term] == 0){
                c->y_coef[*c_term] = c->x_coef[*c_term] * c->y_coef[*c_term] * c->z_coef[*c_term];
                c->x_coef[*c_term] = 1;
                c->z_coef[*c_term] = 1;
            }
            else if(c->y_degree[*c_term] == 0 && c->z_degree[*c_term] == 0){
                c->x_coef[*c_term] = c->x_coef[*c_term] * c->y_coef[*c_term] * c->z_coef[*c_term];
                c->y_coef[*c_term] = 1;
                c->z_coef[*c_term] = 1;
            }
            (*c_term)++;
        }
    }

}

void result_print(polynomial *c, int c_term){
    for(int i=0;i<=c_term;i++){
        //printf("Term %dth: ", i);
        // 부호 출력 파트
        if(c->x_coef[i] * c->y_coef[i] * c->z_coef[i] >= 0){
            // 양수
            // 첫 항은 +부호를 출력하지 않으므로 i!=0 일 때만 + 출력
            if(i!=0) printf("+");
        }
        else{
            // 음수
            printf("-");
        }

        // Y항 출력 파트
        if(c->y_coef[i] == 1 && c->y_degree[i] == 0){
            // Y항 없음
        }
        else{
            // Y항 계수 출력 파트
            if(c->y_coef[i] == 1){
                // 계수 1일 경우 생략
            }
            else{
                // 부호 이미 출력했으므로 계수는 절댓값으로 출력
                if(c->y_coef[i]<0) printf("%d", abs(c->y_coef[i]));
                else printf("%d", c->y_coef[i]);
            }
        }
        // Y항 문자 및 차수 출력 파트
        if(c->y_degree[i] == 0){
            
        }
        else{
            printf("y^%d", c->y_degree[i]);
        }
        ///////////////////////////////////////////////////////////////////////////////

        // X항 출력 파트
        if(c->x_coef[i] == 1 && c->x_degree[i] == 0){
            // X항 없음
        }
        else{
            // 앞에 Y항이 있었다면 X항 내용을 출력하기 전에 *를 출력함
            if(c->y_coef[i] != 1 || c->y_degree[i] != 0){
                // 둘 중에 하나라도 default 값이 아니면 y항이 있었다는 뜻이므로
                // *출력
                printf("*");
            }
            // X항 계수 출력 파트
            if(c->x_coef[i] == 1){
                // 계수 1일 경우 생략
            }
            else{
                // 부호 이미 출력했으므로 계수는 절댓값으로 출력
                if(c->x_coef[i]<0) printf("%d", abs(c->x_coef[i]));
                else printf("%d", c->x_coef[i]);
            }
        }
        // X항 문자 및 차수 출력 파트
        if(c->x_degree[i] == 0){
            
        }
        else{
            printf("x^%d", c->x_degree[i]);
        }
        ///////////////////////////////////////////////////////////////////////////

        // Z항 출력 파트
        if(c->z_coef[i] == 1 && c->z_degree[i] == 0){
            // Z항 없음
        }
        else{
            // 앞에 Y항 또는 X항이 있었다면 Z항 내용을 출력하기 전에 *를 출력함
            if(c->y_coef[i] != 1 || c->y_degree[i] != 0 || c->x_coef[i] != 1 || c->x_degree[i] != 0){
                // 넷 중에 하나라도 default 값이 아니면 y항 또는 x항이 있었다는 뜻이므로
                // *출력
                printf("*");
            }
            // Z항 계수 출력 파트
            if(c->z_coef[i] == 1){
                // 계수 1일 경우 생략
            }
            else{
                // 부호 이미 출력했으므로 계수는 절댓값으로 출력
                if(c->z_coef[i]<0) printf("%d", abs(c->z_coef[i]));
                else printf("%d", c->z_coef[i]);
            }
        }
        // Z항 문자 및 차수 출력 파트
        if(c->z_degree[i] == 0){
            
        }
        else{
            printf("z^%d", c->z_degree[i]);
        }
        //printf("\n");
    }
}
int main(){
    polynomial a;
    polynomial b;
    polynomial c;
    char a_str[100];
    char b_str[100];
    char a_str_norm[100];
    char b_str_norm[100];
    int a_term = 0;
    int b_term = 0;
    int c_term = 0;
    printf("A: ");
    gets(a_str);
    printf("B: ");
    gets(b_str);
    
    // 다항식 리셋
    polynomial_reset(&a);
    polynomial_reset(&b);
    polynomial_reset(&c);
    normalize(a_str, a_str_norm);
    normalize(b_str, b_str_norm);

    a_term = parser(&a, a_str_norm);
    b_term = parser(&b, b_str_norm);

    multiple(&a, &b, &c, a_term, b_term, &c_term);
    sort(&c, c_term-1);
    //print_polynomial(&c, c_term-1);
    result_print(&c, c_term-1);
    return 0;
}
