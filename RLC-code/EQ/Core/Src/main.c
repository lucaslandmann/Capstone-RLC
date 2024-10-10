/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define N 32
#define halfN 16
#define NUMSTAGES 2
#define SNR_THRESHOLD 98
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t adc_buffer[N];
uint32_t dac_buffer[N];


float32_t filt_in[N];
float32_t filt_out[N];
float32_t *filt_in_ptr = &filt_in[0];
float32_t *filt_out_ptr = &filt_out[0];

static q63_t biquadStateBand1Q31[4 * 2];
static q63_t biquadStateBand2Q31[4 * 2];
static q31_t biquadStateBand3Q31[4 * 2];
static q31_t biquadStateBand4Q31[4 * 2];
static q31_t biquadStateBand5Q31[4 * 2];

q31_t inputQ31[N];
q31_t outputQ31[N];
q31_t *inputQ31_ptr= &inputQ31[0];
q31_t *outputQ31_ptr= &outputQ31[0];


const q31_t coeffTable[950] = {
  /* Band 1, -9 dB gain */
  535576962, -1071153923, 535576962, 1073741824, -536870912, 535576962, -1063501998, 527979313, 1060865294, -524146981,
  /* Band 1, -8 dB gain */
  535723226, -1071446451, 535723226, 1073741824, -536870912, 535723226, -1063568947, 527903217, 1061230578, -524503778,
  535868593, -1071737186, 535868593, 1073741824, -536870912, 535868593, -1063627467, 527819780, 1061585502, -524850686,
  536013181, -1072026363, 536013181, 1073741824, -536870912, 536013181, -1063677598, 527728935, 1061930361, -525187972,
  536157109, -1072314217, 536157109, 1073741824, -536870912, 536157109, -1063719372, 527630607, 1062265438, -525515897,
  536300492, -1072600983, 536300492, 1073741824, -536870912, 536300492, -1063752815, 527524720, 1062591011, -525834716,
  536443447, -1072886894, 536443447, 1073741824, -536870912, 536443447, -1063777945, 527411186, 1062907350, -526144676,
  536586091, -1073172183, 536586091, 1073741824, -536870912, 536586091, -1063794775, 527289917, 1063214717, -526446017,
  536728541, -1073457082, 536728541, 1073741824, -536870912, 536728541, -1063803308, 527160815, 1063513366, -526738975,
  536870912, -1073741824, 536870912, 1073741824, -536870912, 536870912, -1063803543, 527023777, 1063803543, -527023777,
  537013321, -1074026642, 537013321, 1073741824, -536870912, 537013321, -1063795470, 526878696, 1064085490, -527300648,
  537155884, -1074311768, 537155884, 1073741824, -536870912, 537155884, -1063779073, 526725455, 1064359439, -527569803,
  537298718, -1074597435, 537298718, 1073741824, -536870912, 537298718, -1063754328, 526563934, 1064625617, -527831454,
  537441939, -1074883878, 537441939, 1073741824, -536870912, 537441939, -1063721205, 526394005, 1064884245, -528085806,
  537585666, -1075171331, 537585666, 1073741824, -536870912, 537585666, -1063679666, 526215534, 1065135536, -528333059,
  537730015, -1075460030, 537730015, 1073741824, -536870912, 537730015, -1063629666, 526028380, 1065379699, -528573409,
  537875106, -1075750212, 537875106, 1073741824, -536870912, 537875106, -1063571152, 525832396, 1065616936, -528807045,
  538021057, -1076042114, 538021057, 1073741824, -536870912, 538021057, -1063504065, 525627429, 1065847444, -529034151,
  538167989, -1076335977, 538167989, 1073741824, -536870912, 538167989, -1063428338, 525413317, 1066071412, -529254907,
  /* Band 2, -9 dB gain */
  531784976, -1055497692, 523873415, 1066213307, -529420241, 531784976, -1040357886, 509828014, 1028908252, -494627367,
  /* Band 2, -8 dB gain */
  532357636, -1056601982, 524400080, 1066115844, -529326645, 532357636, -1040623406, 509562600, 1030462237, -496062122,
  532927392, -1057707729, 524931110, 1066024274, -529239070, 532927392, -1040848253, 509262081, 1031969246, -497457090,
  533494678, -1058816094, 525467240, 1065939047, -529157961, 533494678, -1041032161, 508925950, 1033429976, -498812573,
  534059929, -1059928204, 526009170, 1065860582, -529083734, 534059929, -1041174868, 508553717, 1034845124, -500128887,
  534623580, -1061045148, 526557561, 1065789260, -529016764, 534623580, -1041276126, 508144920, 1036215393, -501406373,
  535186068, -1062167969, 527113032, 1065725420, -528957385, 535186068, -1041335703, 507699125, 1037541500, -502645399,
  535747827, -1063297666, 527676151, 1065669351, -528905879, 535747827, -1041353386, 507215934, 1038824183, -503846368,
  536309295, -1064435183, 528247436, 1065621289, -528862476, 536309295, -1041328990, 506694984, 1040064203, -505009724,
  536870912, -1065581413, 528827349, 1065581413, -528827349, 536870912, -1041262354, 506135953, 1041262354, -506135953,
  537433117, -1066737194, 529416295, 1065549847, -528800610, 537433117, -1041153346, 505538564, 1042419457, -507225588,
  537996352, -1067903307, 530014622, 1065526651, -528782316, 537996352, -1041001864, 504902578, 1043536370, -508279208,
  538561061, -1069080480, 530622620, 1065511830, -528772462, 538561061, -1040807833, 504227800, 1044613981, -509297437,
  539127690, -1070269387, 531240527, 1065505333, -528770987, 539127690, -1040571205, 503514074, 1045653211, -510280946,
  539696690, -1071470656, 531868525, 1065507054, -528777778, 539696690, -1040291951, 502761277, 1046655011, -511230450,
  540268512, -1072684867, 532506750, 1065516837, -528792672, 540268512, -1039970063, 501969320, 1047620358, -512146700,
  540843613, -1073912567, 533155297, 1065534483, -528815459, 540843613, -1039605542, 501138139, 1048550251, -513030484,
  541422451, -1075154268, 533814224, 1065559750, -528845892, 541422451, -1039198394, 500267687, 1049445708, -513882621,
  542005489, -1076410460, 534483561, 1065592362, -528883686, 542005489, -1038748624, 499357932, 1050307760, -514703956,
  518903861, -1001986830, 486725277, 1037235801, -502367695, 518903861, -945834422, 446371043, 902366163, -400700571,
  520899989, -1005630916, 488289126, 1036926846, -502147311, 520899989, -946490935, 445581846, 907921945, -404936158,
  522893209, -1009290002, 489869792, 1036650484, -501961419, 522893209, -947006359, 444685310, 913306106, -409075225,
  524884763, -1012968199, 491470256, 1036407567, -501810737, 524884763, -947377809, 443679533, 918521018, -413116221,
  526875910, -1016669649, 493093518, 1036198712, -501695739, 526875910, -947602324, 442562672, 923569247, -417057897,
  528867927, -1020398503, 494742575, 1036024293, -501616651, 528867927, -947676875, 441332970, 928453558, -420899319,
  530862111, -1024158905, 496420407, 1035884447, -501573457, 530862111, -947598385, 439988777, 933176909, -424639872,
  532859778, -1027954970, 498129955, 1035779077, -501565907, 532859778, -947363742, 438528571, 937742446, -428279254,
  534862260, -1031790763, 499874098, 1035707863, -501593525, 534862260, -946969823, 436950987, 942153486, -431817474,
  536870912, -1035670279, 501655630, 1035670279, -501655630, 536870912, -946413508, 435254839, 946413508, -435254839,
  538887107, -1039597419, 503477238, 1035665609, -501751354, 538887107, -945691703, 433439146, 950526127, -438591937,
  540912240, -1043575967, 505341475, 1035692963, -501879659, 540912240, -944801359, 431503152, 954495080, -441829621,
  542947726, -1047609569, 507250741, 1035751307, -502039364, 542947726, -943739490, 429446349, 958324201, -444968987,
  544995000, -1051701717, 509207261, 1035839473, -502229165, 544995000, -942503190, 427268492, 962017400, -448011351,
  547055523, -1055855728, 511213065, 1035956193, -502447657, 547055523, -941089647, 424969617, 965578640, -450958226,
  549130774, -1060074734, 513269973, 1036100110, -502693359, 549130774, -939496155, 422550049, 969011913, -453811298,
  551222259, -1064361672, 515379585, 1036269804, -502964731, 551222259, -937720119, 420010407, 972321228, -456572401,
  553331507, -1068719280, 517543273, 1036463810, -503260192, 553331507, -935759057, 417351601, 975510582, -459243495,
  555460072, -1073150100, 519762181, 1036680633, -503578144, 555460072, -933610600, 414574832, 978583948, -461826644,
  494084017, -851422604, 404056273, 930151631, -423619864, 494084017, -673714108, 339502486, 561843007, -265801750,
  498713542, -859177141, 406587077, 929211656, -423786402, 498713542, -673274906, 338185129, 573719128, -272222942,
  503369016, -867012190, 409148384, 928362985, -424054784, 503369016, -672533059, 336693984, 585290277, -278599028,
  508052536, -874935599, 411746438, 927604291, -424422151, 508052536, -671478538, 335026905, 596558312, -284920289,
  512766286, -882955583, 414387826, 926933782, -424885216, 512766286, -670100998, 333182045, 607525792, -291177811,
  517512534, -891080712, 417079474, 926349262, -425440318, 517512534, -668389789, 331157902, 618195914, -297363485,
  522293635, -899319903, 419828635, 925848177, -426083491, 522293635, -666333963, 328953368, 628572440, -303470012,
  527112032, -907682405, 422642886, 925427679, -426810526, 527112032, -663922286, 326567785, 638659631, -309490882,
  531970251, -916177781, 425530105, 925084675, -427617023, 531970251, -661143261, 324000998, 648462180, -315420352,
  536870912, -924815881, 428498454, 924815881, -428498454, 536870912, -657985147, 321253420, 657985147, -321253420,
  541816719, -933606817, 431556352, 924617870, -429450209, 541816719, -654435997, 318326093, 667233900, -326985786,
  546810467, -942560921, 434712438, 924487114, -430467639, 546810467, -650483688, 315220754, 676214053, -332613816,
  551855042, -951688708, 437975532, 924420027, -431546101, 551855042, -646115970, 311939896, 684931422, -338134495,
  556953421, -961000826, 441354588, 924413001, -432680993, 556953421, -641320513, 308486839, 693391970, -343545389,
  562108672, -970508005, 444858642, 924462435, -433867780, 562108672, -636084967, 304865786, 701601770, -348844597,
  567323959, -980220994, 448496743, 924564764, -435102022, 567323959, -630397020, 301081886, 709566963, -354030710,
  572602539, -990150500, 452277894, 924716482, -436379394, 572602539, -624244471, 297141281, 717293726, -359102767,
  577947763, -1000307125, 456210977, 924914158, -437695705, 577947763, -617615296, 293051155, 724788245, -364060214,
  583363084, -1010701292, 460304674, 925154455, -439046908, 583363084, -610497723, 288819761, 732056685, -368902865,
  387379495, -506912469, 196933274, 840112184, -347208270, 387379495, 506912469, 196933274, -840112184, -347208270,
  401658082, -532275898, 207149427, 833765363, -343175316, 401658082, 532275898, 207149427, -833765363, -343175316,
  416472483, -558722695, 217902617, 827270154, -339107319, 416472483, 558722695, 217902617, -827270154, -339107319,
  431841949, -586290861, 229212798, 820624988, -335007540, 431841949, 586290861, 229212798, -820624988, -335007540,
  447786335, -615019650, 241100489, 813828443, -330879528, 447786335, 615019650, 241100489, -813828443, -330879528,
  464326111, -644949597, 253586805, 806879270, -326727141, 464326111, 644949597, 253586805, -806879270, -326727141,
  481482377, -676122557, 266693475, 799776409, -322554559, 481482377, 676122557, 266693475, -799776409, -322554559,
  499276882, -708581728, 280442865, 792519013, -318366296, 499276882, 708581728, 280442865, -792519013, -318366296,
  517732032, -742371685, 294857996, 785106465, -314167221, 517732032, 742371685, 294857996, -785106465, -314167221,
  536870912, -777538408, 309962566, 777538408, -309962566, 536870912, 777538408, 309962566, -777538408, -309962566,
  556717294, -814129313, 325780968, 769814766, -305757943, 556717294, 814129313, 325780968, -769814766, -305757943,
  577295658, -852193284, 342338310, 761935777, -301559360, 577295658, 852193284, 342338310, -761935777, -301559360,
  598631206, -891780698, 359660433, 753902014, -297373230, 598631206, 891780698, 359660433, -753902014, -297373230,
  620749877, -932943463, 377773927, 745714425, -293206383, 620749877, 932943463, 377773927, -745714425, -293206383,
  643678365, -975735041, 396706151, 737374355, -289066077, 643678365, 975735041, 396706151, -737374355, -289066077,
  667444134, -1020210487, 416485252, 728883588, -284960004, 667444134, 1020210487, 416485252, -728883588, -284960004,
  692075438, -1066426476, 437140179, 720244375, -280896294, 692075438, 1066426476, 437140179, -720244375, -280896294,
  717601336, -1114441339, 458700704, 711459472, -276883515, 717601336, 1114441339, 458700704, -711459472, -276883515,
  744051710, -1164315096, 481197437, 702532174, -272930673, 744051710, 1164315096, 481197437, -702532174, -272930673
};

//desired gains for each band
int gainDB[5] = {0, -3, 6, 4, -6};
float32_t snr;

arm_biquad_cas_df1_32x64_ins_q31 S1;
arm_biquad_cas_df1_32x64_ins_q31 S2;
arm_biquad_casd_df1_inst_q31 S3;
arm_biquad_casd_df1_inst_q31 S4;
arm_biquad_casd_df1_inst_q31 S5;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	int i;
	int32_t status;

//	 arm_biquad_cas_df1_32x64_init_q31(&S1, NUMSTAGES,
//	            (q31_t *) &coeffTable[190*0 + 10*(gainDB[0] + 9)],
//	            &biquadStateBand1Q31[0], 2);
//	  arm_biquad_cas_df1_32x64_init_q31(&S2, NUMSTAGES,
//	            (q31_t *) &coeffTable[190*1 + 10*(gainDB[1] + 9)],
//	            &biquadStateBand2Q31[0], 2);
//	  arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
//	          (q31_t *) &coeffTable[190*2 + 10*(gainDB[2] + 9)],
//	          &biquadStateBand3Q31[0], 2);
//	  arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
//	          (q31_t *) &coeffTable[190*3 + 10*(gainDB[3] + 9)],
//	          &biquadStateBand4Q31[0], 2);
//	  arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
//	          (q31_t *) &coeffTable[190*4 + 10*(gainDB[4] + 9)],
//	          &biquadStateBand5Q31[0], 2);


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
   HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc1, adc_buffer, N);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, dac_buffer, N, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim8);
  uint8_t buttonState = 1;
  uint8_t toggled = 0;
  uint8_t toggle = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  buttonState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

	  if(buttonState == 1 && toggled == 0) {
		  toggle = toggle ? 0: 1;
		  toggled = 1;
	  }

	  if(buttonState == 0){
		  toggled = 0;
	  }

	  if (toggle == 1){

	  }
	  else {

	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T8_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 16;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 49;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc1) {

	arm_biquad_cas_df1_32x64_init_q31(&S1, NUMSTAGES,
		          (q31_t *) &coeffTable[190*0 + 10*(gainDB[0] + 9)],
		          &biquadStateBand1Q31[0], 2);
	arm_biquad_cas_df1_32x64_init_q31(&S2, NUMSTAGES,
		          (q31_t *) &coeffTable[190*1 + 10*(gainDB[1] + 9)],
		           &biquadStateBand2Q31[0], 2);
	 arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
		          (q31_t *) &coeffTable[190*2 + 10*(gainDB[2] + 9)],
		          &biquadStateBand3Q31[0], 2);
	arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
		          (q31_t *) &coeffTable[190*3 + 10*(gainDB[3] + 9)],
		          &biquadStateBand4Q31[0], 2);
	arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
		          (q31_t *) &coeffTable[190*4 + 10*(gainDB[4] + 9)],
		          &biquadStateBand5Q31[0], 2);



	for( int n=0; n<halfN; n++){ filt_in[n] = (float32_t) adc_buffer[n];}
	//arm_fir_f32(&filter1, filt_in_ptr, filt_out_ptr, halfN);

	/* ----------------------------------------------------------------------
	** Convert block of input data from float to Q31
	** ------------------------------------------------------------------- */
	for (int i = 0; i < 16; i++) {


		arm_float_to_q31(filt_in_ptr + (i * 32), inputQ31, 32);


		/* ----------------------------------------------------------------------
		** Scale down by 1/8.  This provides additional headroom so that the
		** graphic EQ can apply gain.
		** ------------------------------------------------------------------- */
		arm_scale_q31(inputQ31, 0x7FFFFFFF, -3, inputQ31, 32);

		/* ----------------------------------------------------------------------
		** Call the Q31 Biquad Cascade DF1 32x64 process function for band1, band2
		** ------------------------------------------------------------------- */
		arm_biquad_cas_df1_32x64_q31(&S1, inputQ31, outputQ31, 32);
		//arm_biquad_cas_df1_32x64_q31(&S2, outputQ31, outputQ31, halfN);
		/* ----------------------------------------------------------------------
		** Call the Q31 Biquad Cascade DF1 process function for band3, band4, band5
		** ------------------------------------------------------------------- */
		//arm_biquad_cascade_df1_q31(&S3, outputQ31, outputQ31, halfN);
		//arm_biquad_cascade_df1_q31(&S4, outputQ31, outputQ31, halfN);
		//arm_biquad_cascade_df1_q31(&S5, outputQ31, outputQ31, halfN);
		/* ----------------------------------------------------------------------
		** Convert Q31 result back to float
		** ------------------------------------------------------------------- */
		arm_q31_to_float(outputQ31, filt_out_ptr + (i * 32), 32);


		/* ----------------------------------------------------------------------
		** Scale back up
		** ------------------------------------------------------------------- */
		arm_scale_f32(filt_out_ptr + (i + 32), 8.0f, filt_out_ptr + (i + 32, 32), 32);
	}


//	for( int n=0; n<halfN; n++)		{
//		filt_out[n] += 2;
//		dac_buffer[n] = (uint32_t) (filt_out[n]);
//	}

}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1) {
//
//	arm_biquad_cas_df1_32x64_init_q31(&S1, NUMSTAGES,
//		            (q31_t *) &coeffTable[190*0 + 10*(gainDB[0] + 9)],
//		            &biquadStateBand1Q31[0], 2);
//		  arm_biquad_cas_df1_32x64_init_q31(&S2, NUMSTAGES,
//		            (q31_t *) &coeffTable[190*1 + 10*(gainDB[1] + 9)],
//		            &biquadStateBand2Q31[0], 2);
//		  arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
//		          (q31_t *) &coeffTable[190*2 + 10*(gainDB[2] + 9)],
//		          &biquadStateBand3Q31[0], 2);
//		  arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
//		          (q31_t *) &coeffTable[190*3 + 10*(gainDB[3] + 9)],
//		          &biquadStateBand4Q31[0], 2);
//		  arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
//		          (q31_t *) &coeffTable[190*4 + 10*(gainDB[4] + 9)],
//		          &biquadStateBand5Q31[0], 2);
//
//	for( int n=halfN; n<N; n++){filt_in[n] = (float32_t) adc_buffer[n];}
//
//	//filt_in[n] = (float32_t) adc_buffer[n];
//
//	/* ----------------------------------------------------------------------
//	** Convert block of input data from float to Q31
//	** ------------------------------------------------------------------- */
//	for (int i = 16; i < 32; i++) {
//
//		arm_float_to_q31(filt_in_ptr + (i * 32), inputQ31, 32);
//
//	/* ----------------------------------------------------------------------
//	** Scale down by 1/8.  This provides additional headroom so that the
//	** graphic EQ can apply gain.
//	** ------------------------------------------------------------------- */
//		arm_scale_q31(inputQ31, 0x7FFFFFFF, -3, inputQ31, 32);
//	/* ----------------------------------------------------------------------
//	** Call the Q31 Biquad Cascade DF1 32x64 process function for band1, band2
//	** ------------------------------------------------------------------- */
//		arm_biquad_cas_df1_32x64_q31(&S1, inputQ31, outputQ31, 32);
//	//arm_biquad_cas_df1_32x64_q31(&S2, outputQ31 + halfN, outputQ31 + halfN, halfN);
//	/* ----------------------------------------------------------------------
//	** Call the Q31 Biquad Cascade DF1 process function for band3, band4, band5
//	** ------------------------------------------------------------------- */
//	//arm_biquad_cascade_df1_q31(&S3, outputQ31 + halfN, outputQ31 + halfN, halfN);
//	//arm_biquad_cascade_df1_q31(&S4, outputQ31 + halfN, outputQ31 + halfN, halfN);
//	//arm_biquad_cascade_df1_q31(&S5, outputQ31 + halfN, outputQ31, halfN);
//	/* ----------------------------------------------------------------------
//	** Convert Q31 result back to float
//	** ------------------------------------------------------------------- */
//		arm_q31_to_float(outputQ31, filt_out_ptr + (i * 32), 32);
//	/* ----------------------------------------------------------------------
//	** Scale back up
//	** ------------------------------------------------------------------- */
//		arm_scale_f32(filt_out_ptr + (i * 32), 8.0f, filt_out_ptr + (i * 32), 32);
//
//	}
//
//
//	//arm_fir_f32(&filter1, filt_in_ptr + halfN, filt_out_ptr + halfN, halfN);
////    for( int n=halfN; n<N; n++){
////    	filt_out[n] += 2;
////    	dac_buffer[n] = (uint32_t) (filt_out[n] * 1000);}
//
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
