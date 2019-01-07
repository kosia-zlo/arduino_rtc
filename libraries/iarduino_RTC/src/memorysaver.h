//	Файл: «ХРАНИТЕЛЬ ПАМЯТИ»

//	Если закомментировать строки с названиями тех модулей которые не используются в Вашем устройстве, можно освободить около 50 байт флеш-памяти
//	Если закомментировать строку с названием используемого модуля, компилятор выдаст ошибку

	//#define	RTC_DISABLE_DS1302		1	//	Модуль часов реального времени (RTC) на базе чипа DS1302, с протоколом передачи данных SI3, памятью 040x8 (31 байт которой доступны для хранения данных пользователя)
	#define	RTC_DISABLE_DS1307		1	//	Модуль часов реального времени (RTC) на базе чипа DS1307, с протоколом передачи данных I2C, памятью 064x8 (56 байт которой доступны для хранения данных пользователя)
	//#define RTC_DISABLE_DS3231		1	//	Модуль часов реального времени (RTC) на базе чипа DS3231, с протоколом передачи данных I2C, памятью 019x8, температурной компенсацией, двумя будильниками и встроенным кварцевым резонатором