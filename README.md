# Mini-Shell
Ⅰ. 구현한 명령어 목록

![image](https://user-images.githubusercontent.com/29851772/200361153-daaf106b-94ac-4898-b37f-781928937f06.png)

Ⅱ. 구현한 명령어 코드 설명

- main() 함수

![image](https://user-images.githubusercontent.com/29851772/200360904-d4c8b18c-c639-455a-8826-1f1a546673c2.png)

main함수에서 여러 변수들을 선언 및 초기화를 해준다.\
사진에서 위쪽은 전체적인 명령어를 입력받고 처리하는데 쓰이는 변수,\
중간엔 history에 관한 변수, 아래쪽은 alias에 관한 변수이다.

![image](https://user-images.githubusercontent.com/29851772/200361037-ed278a5b-5210-4bb1-a3d0-474db24a8a79.png)

그 다음 무한루프의 while문이 시작된다.\
print_prompt(); 함수는 현재 working directory 경로를 얻어와 출력해주는 함수이다.\
fgets() 함수로 buf 변수에 명령어를 입력 받고 첫 글자가 ‘!’ 인지 확인하는과정을 거친다.\
만약 ‘!’ 라면 !를 뺀 뒷 글자를 숫자로 바꾼 후 history 목록에서 찾아 해당하는 명령어를 buf에 넣어준다.

buf에 입력되어있는 명령어를 history 목록에 넣어준다.\
그리고 명령어를 parsing 하는 과정을 거친다. 공백을 기준으로 arg[0], arg[1] … 순서로 저장된다.

혹시 입력된 문자열이 alias 목록에 있는지 검사해보고, 있다면 alias를 원래 명령어로 바꿔주는 과정을 거친다.

![image](https://user-images.githubusercontent.com/29851772/200361271-cb4ef13e-9f26-4d71-b06e-29b46c68db2f.png)\
![image](https://user-images.githubusercontent.com/29851772/200361319-d17ba74e-c422-468b-b1f9-76dcc22da658.png)

그 다음부터 실제 명령어 처리가 시작되는데 arg[0]에 저장된 문자열이 어떤 것 인지에 따라서 어떤 함수를 실행할지를 나눠준다.\
입력받은 명령어가 저 목록중에 없다면 에러처리를 해줬다. 
exit 명령어는 따로 함수를 만들지 않고 break로 shell이 종료되게 처리했다.

- 구현한 명령어 함수

![image](https://user-images.githubusercontent.com/29851772/200362475-a2479784-b7e7-46f9-9f8b-96f419f8035a.png)

my_cat() 함수는 파일명을 매개변수로 open 함수를 이용해 읽기 전용 모드로 파일을 연다.\
실패하면 에러처리 하고, 성공하면 파일 내용을 한글자씩 읽어와서 출력한다.

![image](https://user-images.githubusercontent.com/29851772/200362722-57eb24a5-3b8c-4e95-992d-7229ae4e3e2c.png)

my_pwd() 함수는 getcwd()를 사용해 현재 working directory 경로를 가져오고 출력한다.

![image](https://user-images.githubusercontent.com/29851772/200362842-cbf8f42f-0daf-4047-9b3e-4281bbd0d2cd.png)

my_cd() 함수는 chdir()를 사용해 working directory를 바꿔주고 성공하면 바뀐 경로를 출력하고 실패하면 에러처리 한다.

![image](https://user-images.githubusercontent.com/29851772/200362898-117e5ea9-22b9-4637-adbd-30f18ec46770.png)

my_mkdir() 함수는 mkdir()을 이용해 입력받은 이름으로 directory를 생성한다.

![image](https://user-images.githubusercontent.com/29851772/200362949-bdf9c67e-666b-4bc9-91e1-7a2dc7cdb46c.png)

my_ls() 함수는 현재 wordking directory 경로를 가져온다.\
입력받은 명령어 중 ls 뒤에 뭔가 있었다면 그 경로를 추가시켜주고 ls만 입력받았다면 그 경로로 opendir()를 한다.\
NULL이 반환되면 에러처리 하고, NULL이 아니라면 directory안의 파일명 혹은 directory명을 하나씩 읽어 내려간다.\
첫글자가 ‘.’ 이면 숨김파일이므로 첫글자가 ‘.’ 이 아닌 것만 출력해준다.

#################my_ps() 함수 이미지#################

my_ps() 함수는 “/proc” 경로의 directory 안에 있는 파일을 읽어내려온다.\
프로세스는 /proc에 자신의 pid로 directory를 만들기 때문에 directory인지 확인하는 과정이 있다.\
directory라는 것을 확인하면 pid를 가져오고 pid가 숫자가 아니면 continue.\
/proc/pid/status 와 /proc/pid/stat 두 경로를 설정하고 각각의 파일을 읽는다.\
/proc/pid/status 경로의 파일을 fgets로 한줄씩 읽어내려오는데 9번째 줄이 uid를 나타내기 때문에 9번째 줄을 parsing 해준다.\
parsing으로 얻은 uid값과 getuid()함수를 사용해 얻은 uid 값이 같은 파일이라면 /proc/pid/stat 경로의 파일을 읽어 괄호를 떼고 pid값과 함께 CMD값으로 출력한다.

![image](https://user-images.githubusercontent.com/29851772/200364650-b7a1ad84-df83-46c1-9416-68c904074000.png)

my_cp() 함수는 파일명 2개를 받아와서 첫 번째 파일을 열어본다.\
열리지 않으면 에러처리하고, 열렸다면 두 번째 파일을 열어 첫 번째 파일의 내용을 읽고 두 번째 파일에 쓰는 작업을 한글자씩 반복한다.\
만약 두 번째 파일이 존재하지 않는다면 새로 생성한다.

![image](https://user-images.githubusercontent.com/29851772/200364790-6ab036bf-d088-4705-b93f-e65113ccf694.png)

my_rm() 함수는 입력받은 함수 혹은 디렉토리를 remove() 함수를 사용해 삭제한다.\
디렉토리는 비어있지 않다면 삭제되지 않는다.

my_ln() 함수는 첫 번째 입력받은 파일의 hardlink 파일을 두 번째 입력받은 파일명으로 생성한다.


#################my_stat() 함수 이미지#################
stat 함수 설명

![image](https://user-images.githubusercontent.com/29851772/200365210-2cba3673-7fd6-42af-9918-3fc1f6846e82.png)

my_chmod() 함수는 첫 번째 매개변수로 입력받은 모드를, 두 번째 매개변수로 입력받은 파일 명을 사용한다.\
먼저 파일이 있는지 확인해보고 없다면 파일이 존재하지 않는다는 에러처리를 한다.\
그리고 입력받은 숫자를 8진수로 바꿔주고 chmod() 함수를 사용해 파일의 permission을 변경 해준다.

![image](https://user-images.githubusercontent.com/29851772/200365408-548a2954-3087-47d6-b862-beb1daf6f53c.png)

hitory 기능은 history 구조체 배열에 입력받은 명령어를 추가하는 input_history() 함수와\
history 목록을 출력하는 my_history() 함수 두 가지로 구현된다.

![image](https://user-images.githubusercontent.com/29851772/200365418-24499d0b-a6d2-4af1-ad51-b078894620db.png)

alias 기능도 history와 마찬가지로 input_alias() 함수와 my_alias() 함수 두 가지가 있다.\
input_alias() 함수는 main함수에서 parsing되어 넘어온 alias부분과 명령어 부분을 각각 aliasList 구조체 배열에 저장한다.\
my_alias() 함수는 alias 목록을 출력해준다.


