/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "nes_main.h"
#include "lcd.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8 Continue = TRUE;//初始化为真
int FrameCnt;


#include "fuck_joypad.h"



/* NES 帧周期循环*/   
void NesFrameCycle(void)
{
 	int	clocks;	//CPU执行时间

//// 启动模拟器循环，检查VROM设置，若卡带为0，初始化VROM
//	if ( NesHeader.byVRomSize == 0)
//		设置VROM存储区位置。
	FrameCnt = 0;
	while(Continue){
		/* scanline: 0~19 VBANK 段，若PPU使能NMI，将产生NMI 中断, */
		FrameCnt++;		   //帧计数器
//		printf("\r\n当前显示帧 %d", FrameCnt);
		SpriteHitFlag = FALSE;		
		for(PPU_scanline=0; PPU_scanline<20; PPU_scanline++){ 
			exec6502(CLOCKS_PER_SCANLINE);
//			NesHBCycle();
		}
		/* scanline: 20, PPU加载设置，执行一次空的扫描时间*/
		exec6502(CLOCKS_PER_SCANLINE);
//		NesHBCycle();  //水平周期
		PPU_scanline++;	  //20++
		PPU_Reg.R2 &= ~R2_SPR0_HIT;
		/* scanline: 21~261*/	
		for(; PPU_scanline < SCAN_LINE_DISPALY_END_NUM; PPU_scanline++){
			if((SpriteHitFlag == TRUE) && ((PPU_Reg.R2 & R2_SPR0_HIT) == 0)){
				clocks = sprite[0].x * CLOCKS_PER_SCANLINE / NES_DISP_WIDTH;
				exec6502(clocks);
				PPU_Reg.R2 |= R2_SPR0_HIT;
				exec6502(CLOCKS_PER_SCANLINE - clocks);
			}else{
			    exec6502(CLOCKS_PER_SCANLINE);
			}

			if(PPU_Reg.R1 & (R1_BG_VISIBLE | R1_SPR_VISIBLE)){					//若为假，关闭显示
				if(SpriteHitFlag == FALSE)
					NES_GetSpr0HitFlag(PPU_scanline - SCAN_LINE_DISPALY_START_NUM);						//查找Sprite #0 碰撞标志
			}
			if(FrameCnt & 2){										 //每2帧显示一次
				NES_RenderLine(PPU_scanline - SCAN_LINE_DISPALY_START_NUM);					//水平同步与显示一行
			}	
		}

		
		/* scanline: 262 完成一帧*/
		exec6502(CLOCKS_PER_SCANLINE);

		

		PPU_Reg.R2 |= R2_VBlank_Flag;	//设置VBANK 标志
		/*若使能PPU VBANK中断，则设置VBANK*/
		if(PPU_Reg.R0 & R0_VB_NMI_EN){
			NMI_Flag = SET1;	//完成一帧扫描，产生NMI中断
		}

	   	/*设置帧IRQ标志，同步计数器，APU等*/
		
		/* A mapper function in V-Sync 存储器切换垂直VBANK同步*/
//		MapperVSync();
		
		/*读取控制器JoyPad状态,更新JoyPad控制器值*/
		// NES_JoyPadUpdateValue();	 //systick 中断读取按键值

		fuck_NES_JoyPadUpdateValue();	 //systick 中断读取按键值


		//LCD_ShowNum(80,50,32,90,24);
		/*设置循环标志，是否退出循环*/
//		if(){
//		 	Continue = FALSE;
//		}
	}

	
}

/**
  * @brief  NES_Main program.
  * @param  None
  * @retval None
  */
#include "lcd.h"

void nes_main(void)
{	



	NesHeader *neshreader = (NesHeader *) rom_file;

/* 	由于NES文件头类型以0x1A结束，（0x1a是Ctrl+Z,这是模拟文件结束的符号）
 *	所以使用strcncmp比较前3个字节。
 */
	// LCD_ShowString(60,50,200,16,16,"fuck");
 //LCD_ShowString(60,50,200,16,16,neshreader->filetype); 
	// if(strncmp(neshreader->filetype, "NES", 3) != 0){
		 
	// 	printf("\r\n文件未加载或文件类型错误, NES模拟器退出。");
	// 	return;
	// }else{


		// printf("\r\n文件加载完成。");
		// printf("\r\n  16kB  ROM 的数目: %d", neshreader->romnum);
		// printf("\r\n   8kB VROM 的数目: %d", neshreader->vromnum);
		// if((neshreader->romfeature & 0x01) == 0){
		// 	printf("\r\n  水平镜像");
		// }else{
	 // 		printf("\r\n  垂直镜像");
		// }

		// if((neshreader->romfeature & 0x02) == 0){
		// 	printf("\r\n  无记忆SRAM");
		// }else{
	 // 		printf("\r\n  有记忆SRAM");
		// }

		// if((neshreader->romfeature & 0x04) == 0){
		// 	printf("\r\n  无512字节的trainer($7000-$71FF)");
		// }else{
	 // 		printf("\r\n  有512字节的trainer(ROM格式暂不支持)");
		// }

		// if((neshreader->romfeature & 0x08) == 0){
		// 	printf("\r\n  2屏幕VRAM布局");
		// }else{
	 // 		printf("\r\n  4屏幕VRAM布局(暂不支持)");
		// }

		// printf("\r\n  iNES Mapper Numbers: %d", (neshreader->rommappernum & 0xF0)|( neshreader->romfeature >> 4));
	// }
	
/*
 *初始化nes 模拟器
 */
	init6502mem( 0,			/*exp_rom*/
				 0,			/*sram 由卡类型决定, 暂不支持*/
				(&rom_file[0x10]),		/*prg_rombank, 存储器大小 由卡类型决定*/
			     neshreader->romnum 	
				);  //初始化6502存储器镜像
	

	reset6502();

	PPU_Init((&rom_file[0x10] + (neshreader->romnum * 0x4000)), (neshreader->romfeature & 0x01));	/*PPU_初始化*/	
	

	


	NES_JoyPadInit();


/*
 *nes 模拟器运行循环
 */
	NesFrameCycle();

	//LCD_ShowString(60,50,200,16,16,neshreader->filetype);
}
  /**
  * @}
  */


/*******************************END OF FILE***********************************/
