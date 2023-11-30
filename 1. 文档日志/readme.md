硬件模块接线：

1. PS2遥控模块（SPI2接口）

```c
#define PS2_PORT		GPIOB

#define PS2_CS_SEL		GPIO_Pin_12		//PB12
#define PS2_CLK			GPIO_Pin_13		//PB13
#define PS2_DO_CMD		GPIO_Pin_14		//PB14
#define PS2_DI_DAT		GPIO_Pin_15		//PB15
```



2. 电机驱动模块（麦轮运动）

```c
/*
左后MotorA(A1,A2): PB1,PB0	TIM3_43
左前MotorB(B1,B2): PA7,PA6	TIM3_21
右前MotorC(C1,C2): PA3,PA2	TIM2_43
右后MotorD(D1,D2): PA1,PA0	TIM2_21
*/
#define MOTOR_PORT		GPIOA|GPIOB
///左后电机
#define MOTORA_A1		GPIO_Pin_1		//PB1
#define MOTORA_A2		GPIO_Pin_0		//PB0
///左前电机
#define MOTORB_B1		GPIO_Pin_7		//PA7
#define MOTORB_B2		GPIO_Pin_6		//PA6
///右前电机
#define MOTORC_C1		GPIO_Pin_3		//PA3
#define MOTORC_C2		GPIO_Pin_2		//PA2
///右后电机
#define MOTORD_D1		GPIO_Pin_1		//PA1
#define MOTORD_D2		GPIO_Pin_0		//PA0

```



3. 舵机驱动模块（简易机械臂）

```c
/*
前舵机（夹爪开合）
左舵机（水平轴运动）
右舵机（竖直轴运动）
*/
#define SERVO_PORT		GPIOB

#define SERVO_FRONT		GPIO_Pin_8		//PB8
#define SERVO_LIFT		GPIO_Pin_7		//PB7
#define SERVO_RIGHT		GPIO_Pin_6		//PB6

#define SERVO_ACMAN		GPIO_Pin_9		//PB9

```

<img src="D:\Users\JESON\Desktop\正点精英版_野火_江科大\【实战项目】麦轮遥控小车+机械臂\1. 文档日志\readme.assets\image-20231129144718936.png" alt="image-20231129144718936" style="zoom: 50%;" />