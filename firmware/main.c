/*
DO�T�[�{�p�̃v���O����
��H��ɂ͓��͒[�q��10, �o�͒[�q��5����
���̃v���O�����ł́A���͂��ꂽ2�̃f�W�^���M����ϊ����ăT�[�{��PWM�o�͂��s���Ă���
*/


//AVR�̊�{�I�ȃw�b�_�t�@�C��
#include <avr/io.h>
//delay�֌W�̊֐�(_delay_us, _delay_ms)���g�p����̂ɕK�v
#include <util/delay.h>
//CPU�̃N���b�N�����w�肷�� ���̒l������delay�֐��̑҂����Ԃ����肷�� UL��unsigned long�^�̂��Ɓ@�܂�1��̏�����50�i�m�b(5*10^-8)
#define F_CPU 20000000UL
/*
/DDR�� = 0x����; 
�w�肵���|�[�g�̃r�b�g����͂��o�͂������肷��
0������Γ��͂ɁA1������Ώo�͂ɂȂ�
*/

/*
PORT�� = 0x����;
�w�肵���|�[�g�̏o�͂�H���o����L���o�������肷��
DDR�ŏo�͂ɐݒ肳��Ă���r�b�g��1�������H���o�́A0�������L���o�͂���
�܂��ADDR�œ��͂ɐݒ肳��Ă���r�b�g��1������ƁA���̃s���̓v���A�b�v�����
�v���A�b�v�c���̃��C���ɉ����ڑ�����Ă��Ȃ���ԂɂȂ������ɁA���̃��C���̓d�ʂ��uH���x���v�ɌŒ肷�邱��
*/

/*
bit_is_set(register, bit)	�w�背�W�X�^�̎w��̃r�b�g���Z�b�g(1)����Ă��邩�ǂ����`�F�b�N
bit_is_clear(register, bit)	�Z�b�g(1)����Ă���Ă��Ȃ����`�F�b�N
*/

/*
a &= b �c a = a & b
a |= b �c a = a | b
*/

/*
�ϐ��̉��
define
INI,MAX,MIN�@�c�@���ꂼ�ꏉ���l�A�ő�l�A�ŏ��l�@s[]�͂��͈̔͂̒l���Ƃ�
SPD�@�c�@s[]���ω����Ă����X�s�[�h
*/

#define INI {150,150,150,75,200}
#define SPD {1,1,1,1,1}
#define MAX {225,225,225,180,200}
#define MIN {75,75,100,75,75}

int s[5] = INI;
int spd[5] = SPD;
int max[5] = MAX;
int min[5] = MIN;
int i,j;

/*
s[]�̒l������PWM�M�����o�͂���֐�
���߂�PD0�`4�����ׂ�H�ɂ�����Afor����s[]�̒l�ɉ����Ă��ꂼ��L�ɂ��Ă������Ƃ�PWM�̔g�`��\�����Ă���
100us���Ƃɂ�������s���Ă���?

*/
void pwm(void){
	/*
	for(i = 0;i < 920;i++){
		if(i < s[0])
			PORTD |= 0b00000001;
		else
			PORTD &= 0b11111110;
		if(i < s[1])
			PORTD |= 0b00000010;
		else
			PORTD &= 0b11111101;
		if(i < s[2])
			PORTD |= 0b00000100;
		else
			PORTD &= 0b11111011;
		if(i < s[3])
			PORTD |= 0b00001000;
		else
			PORTD &= 0b11110111;
		if(i < s[4])
			PORTD |= 0b00010000;
		else
			PORTD &= 0b11101111;
		_delay_us(5);
	}*/
	PORTD = 0b00011111;
	for(i = 0;i < 1840;i++){
		if(i == s[0])
			PORTD &= 0b11111110;
		if(i == s[1])
			PORTD &= 0b11111101;
		if(i == s[2])
			PORTD &= 0b11111011;
		if(i == s[3])
			PORTD &= 0b11110111;
		if(i == s[4])
			PORTD &= 0b11101111;
	}
}	

/*
s[]�̒l�𑝌�������֐�
MAX,MIN�͈̔͂ŁAa�ɓ��͂�����Α������Ab�ɓ��͂�����Ό�������
*/
void move(int n,int a,int b){
	if(a){
		s[n] += spd[n];
		if(s[n] > max[n])
			s[n] = max[n];
	}
	if(b){
		s[n] -= spd[n];
		if(s[n] < min[n])
			s[n] = min[n];
	}
}

int main(void){
	//PD0�`4���o�͂ɁA���͓��͂�
	DDRB = 0b00000000;
	DDRD = 0b00011111;
	DDRA = 0;
	//�������[�v �ȉ��̏��������s��������
	while(1){
		move(0,bit_is_set(PINB,PB7),bit_is_set(PINB,PB6));
		move(1,bit_is_set(PINB,PB5),bit_is_set(PINB,PB4));
		move(2,bit_is_set(PINB,PB3),bit_is_set(PINB,PB2));
		move(3,bit_is_set(PINB,PB1),bit_is_set(PINB,PB0));
		move(4,bit_is_set(PIND,PD6),bit_is_set(PIND,PD5));
		pwm();
	}
	return 0;
}
