# Snow Bros 모작

DirectX 11과 C++로 제작한 아케이드 게임 **Snow Bros** 모작 프로젝트입니다.
게임에 필요한 저수준 엔진 기능부터 타일맵, 캐릭터 상태, 적 AI, 눈덩이 상호작용까지 직접 구현했습니다.

## 기술 스택

- C++20
- DirectX 11
- HLSL
- Win32 API
- DirectXTex
- Aseprite
- Python
- Visual Studio / MSVC

## 빌드 방법

### 요구 환경

- Windows 10 이상
- Visual Studio 2026 또는 `v145` 플랫폼 도구 집합을 지원하는 Visual Studio
- Desktop development with C++ 워크로드
- Windows 10 SDK

빌드에 필요한 헤더와 라이브러리는 `Arcade_pky/ThirdParty`에 포함되어 있습니다.

### Visual Studio에서 빌드

1. `Arcade_pky/Arcade_pky.slnx`를 Visual Studio로 엽니다.
2. 빌드 구성을 `Debug | x64` 또는 `Release | x64`로 선택합니다.
3. `Client` 프로젝트를 시작 프로젝트로 설정합니다.
4. 솔루션을 빌드한 뒤 디버깅을 시작합니다.

빌드 결과는 다음 경로에 생성됩니다.

```text
Arcade_pky/Builds/<Configuration>/Client.exe
```

리소스 경로가 실행 위치를 기준으로 설정되므로 단독 실행보다 Visual Studio에서 실행하는 방식을 권장합니다.

## 구현 범위

### 게임 콘텐츠

- 3개 스테이지와 스테이지 전환
- 1인 및 로컬 2인 플레이
- 이동, 점프, 발사, 사망, 부활, 스테이지 클리어 상태
- 적을 눈덩이로 만들고 밀거나 발사하는 핵심 게임플레이
- 일반 적과 보스
- Speed, Power, Range, 무적 등 아이템 효과
- 점수, 하이스코어, 스테이지 및 플레이어 UI

### 엔진 및 시스템

- Actor/Component 기반 게임 객체 구조
- AI 및 플레이어 상태 머신
- AABB와 Point Collider 기반 충돌 처리
- 타일맵 렌더링과 플랫폼 이동/충돌
- Sprite/Tile Instancing
- 인덱스 텍스처와 팔레트 기반 Color Swapping
- 애니메이션과 Animation Notify
- 입력 Context/Action 시스템
- 바이너리 기반 스테이지, 팔레트, 타일 메타데이터 및 애니메이션 데이터 로딩
- 눈덩이 목록을 프레임 단위로 공유하여 재귀 충돌 처리 중 중복 탐색 제거

적 4종과 보스 1종을 구현했습니다.

## 조작키

### 플레이어 1

| 동작 | 키 |
|---|---|
| 왼쪽 이동 | `A` |
| 오른쪽 이동 | `D` |
| 점프 | `T` |
| 눈 발사 | `R` |

### 플레이어 2

| 동작 | 키 |
|---|---|
| 왼쪽 이동 | `H` |
| 오른쪽 이동 | `K` |
| 점프 | `P` |
| 눈 발사 | `O` |

## 치트키

치트키는 테스트 및 디버깅을 위한 기능입니다.

| 키 | 기능 |
|---|---|
| `1` | 팔레트 0 적용 |
| `2` | 팔레트 1 적용 |
| `3` | 팔레트 2 적용 |
| `4` | Goblin 소환 |
| `5` | Monkey 소환 |
| `6` | Spitter 소환 |
| `7` | Spawn 소환 |
| `8` | Boss 소환 |
| `9` | 플레이어 2 활성화 |
| `Tab` | 카메라 표시 범위 전환 |
| `F1` | 스테이지 1로 이동 |
| `F2` | 스테이지 2로 이동 |
| `F3` | 스테이지 3으로 이동 |
| `F4` | 플레이어 1 Power 아이템 적용 |
| `F5` | 플레이어 1 Speed 아이템 적용 |
| `F6` | 플레이어 1 Range 아이템 적용 |
| `F7` | 스트레스 테스트용 Goblin 50마리 생성 |
| `F8` | 스트레스 테스트용 Goblin 100마리 생성 |
| `F9` | 스트레스 테스트용 Goblin 200마리 생성 |
| `F10` | 스트레스 테스트 Goblin을 한 마리씩 눈덩이로 전환 |
