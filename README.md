# Mini-Shell
Ⅰ. 구현한 명령어 목록

![image](https://user-images.githubusercontent.com/29851772/200361153-daaf106b-94ac-4898-b37f-781928937f06.png)

Ⅱ. 구현한 명령어 코드 설명

- main() 함수

![image](https://user-images.githubusercontent.com/29851772/200545242-632b0e07-b0aa-4be0-853e-8b180edd04cc.png)\
main함수에서 여러 변수들을 선언 및 초기화를 해준다.\
사진에서 위쪽은 전체적인 명령어를 입력받고 처리하는데 쓰이는 변수,\
중간엔 history에 관한 변수, 아래쪽은 alias에 관한 변수이다.

![image](https://user-images.githubusercontent.com/29851772/200545448-8eb92c25-1d65-416e-baf8-71ea32ad62c6.png)\
![image](https://user-images.githubusercontent.com/29851772/200545038-331af1c3-4860-4c6f-a88b-6cc076b8f79b.png)\
그 다음 무한루프의 while문이 시작된다.\
print_prompt(); 함수는 현재 working directory 경로를 얻어와 출력해주는 함수이다.\
fgets() 함수로 buf 변수에 명령어를 입력 받고 첫 글자가 ‘!’ 인지 확인하는과정을 거친다.\
만약 ‘!’ 라면 !를 뺀 뒷 글자를 숫자로 바꾼 후 history 목록에서 찾아 해당하는 명령어를 buf에 넣어준다.

buf에 입력되어있는 명령어를 history 목록에 넣어준다.\
그리고 명령어를 parsing 하는 과정을 거친다. 공백을 기준으로 arg[0], arg[1] … 순서로 저장된다.

혹시 입력된 문자열이 alias 목록에 있는지 검사해보고, 있다면 alias를 원래 명령어로 바꿔주는 과정을 거친다.

![tempsnip](https://user-images.githubusercontent.com/29851772/200549650-93675dae-3d64-498c-bfac-12fc0a56f5ef.png)\
그 다음부터 실제 명령어 처리가 시작되는데 arg[0]에 저장된 문자열이 어떤 것 인지에 따라서 어떤 함수를 실행할지를 나눠준다.\
입력받은 명령어가 저 목록중에 없다면 에러처리를 해줬다. 
exit 명령어는 따로 함수를 만들지 않고 break로 shell이 종료되게 처리했다.

- 구현한 명령어 함수

![image](https://user-images.githubusercontent.com/29851772/200544805-19bf3b13-9f32-479f-8ee1-6a676dcff0cc.png)\
my_cat() 함수는 파일명을 매개변수로 open 함수를 이용해 읽기 전용 모드로 파일을 연다.
실패하면 에러처리 하고, 성공하면 파일 내용을 한글자씩 읽어와서 출력한다.

![image](https://user-images.githubusercontent.com/29851772/200544588-d7d122e8-ae25-42ef-ae36-908117b7456f.png)\
my_pwd() 함수는 getcwd()를 사용해 현재 working directory 경로를 가져오고 출력한다.

![image](https://user-images.githubusercontent.com/29851772/200544491-2238b72f-45cd-4b10-93bf-cd5bf1b40fff.png)\
my_cd() 함수는 chdir()를 사용해 working directory를 바꿔주고 성공하면 바뀐 경로를 출력하고 실패하면 에러처리 한다.

![image](https://user-images.githubusercontent.com/29851772/200544155-e442fec0-313b-46e7-ad61-4cdf694fe08f.png)\
my_mkdir() 함수는 mkdir()을 이용해 입력받은 이름으로 directory를 생성한다.

![image](https://user-images.githubusercontent.com/29851772/200545925-168b9e68-0819-4611-bcd7-81abc8b4f863.png)\
my_ls() 함수는 현재 wordking directory 경로를 가져온다.\
입력받은 명령어 중 ls 뒤에 뭔가 있었다면 그 경로를 추가시켜주고 ls만 입력받았다면 그 경로로 opendir()를 한다.\
NULL이 반환되면 에러처리 하고, NULL이 아니라면 directory안의 파일명 혹은 directory명을 하나씩 읽어 내려간다.\
첫글자가 ‘.’ 이면 숨김파일이므로 첫글자가 ‘.’ 이 아닌 것만 출력해준다.


![img](https://user-images.githubusercontent.com/29851772/200548840-3ec129e7-244d-4fcd-bdd0-226497c2db79.png)\
my_ps() 함수는 “/proc” 경로의 directory 안에 있는 파일을 읽어내려온다.\
프로세스는 /proc에 자신의 pid로 directory를 만들기 때문에 directory인지 확인하는 과정이 있다.\
directory라는 것을 확인하면 pid를 가져오고 pid가 숫자가 아니면 continue.\
/proc/pid/status 와 /proc/pid/stat 두 경로를 설정하고 각각의 파일을 읽는다.\
/proc/pid/status 경로의 파일을 fgets로 한줄씩 읽어내려오는데 9번째 줄이 uid를 나타내기 때문에 9번째 줄을 parsing 해준다.\
parsing으로 얻은 uid값과 getuid()함수를 사용해 얻은 uid 값이 같은 파일이라면 /proc/pid/stat 경로의 파일을 읽어 괄호를 떼고 pid값과 함께 CMD값으로 출력한다.

![image](https://user-images.githubusercontent.com/29851772/200549823-79162d30-fe8f-4a99-90a6-3849a1acf32a.png)\
my_cp() 함수는 파일명 2개를 받아와서 첫 번째 파일을 열어본다.\
열리지 않으면 에러처리하고, 열렸다면 두 번째 파일을 열어 첫 번째 파일의 내용을 읽고 두 번째 파일에 쓰는 작업을 한글자씩 반복한다.\
만약 두 번째 파일이 존재하지 않는다면 새로 생성한다.

![image](https://user-images.githubusercontent.com/29851772/200549996-5f2c669b-8d82-4dda-9742-d0575e58d8bb.png)\
my_rm() 함수는 입력받은 함수 혹은 디렉토리를 remove() 함수를 사용해 삭제한다.\
디렉토리는 비어있지 않다면 삭제되지 않는다.

my_ln() 함수는 첫 번째 입력받은 파일의 hardlink 파일을 두 번째 입력받은 파일명으로 생성한다.

![img2](https://user-images.githubusercontent.com/29851772/200551233-6465ded0-61c0-45e4-96d6-350191a57adf.png)\
my_stat() 함수는 lstat() 함수를 사용해 파일의 상태 및 정보등을 얻어 출력해주는 함수이다.

![image](https://user-images.githubusercontent.com/29851772/200551349-2567018f-5186-4a6e-8de6-cf888df33251.png)\
my_chmod() 함수는 첫 번째 매개변수로 입력받은 모드를, 두 번째 매개변수로 입력받은 파일 명을 사용한다.\
먼저 파일이 있는지 확인해보고 없다면 파일이 존재하지 않는다는 에러처리를 한다.\
그리고 입력받은 숫자를 8진수로 바꿔주고 chmod() 함수를 사용해 파일의 permission을 변경 해준다.

![image](https://user-images.githubusercontent.com/29851772/200551588-3a28823f-d0d8-4adb-9366-2675f64a022d.png)\
hitory 기능은 history 구조체 배열에 입력받은 명령어를 추가하는 input_history() 함수와\
history 목록을 출력하는 my_history() 함수 두 가지로 구현된다.

![image](https://user-images.githubusercontent.com/29851772/200551518-d8604bd9-dc09-4cbd-b52a-9a40db44bf80.png)\
alias 기능도 history와 마찬가지로 input_alias() 함수와 my_alias() 함수 두 가지가 있다.\
input_alias() 함수는 main함수에서 parsing되어 넘어온 alias부분과 명령어 부분을 각각 aliasList 구조체 배열에 저장한다.\
my_alias() 함수는 alias 목록을 출력해준다.


