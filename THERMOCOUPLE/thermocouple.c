/*
*********************************************************************************************************
* HeadFile
*********************************************************************************************************
*/

#include "thermocouple.h"

/*
*********************************************************************************************************
* Const
*********************************************************************************************************
*/

static const float Var_VtoT_K[3][10] =
{
    {0, 2.5173462e1, -1.1662878, -1.0833638, -8.9773540/1e1, -3.7342377/1e1, -8.6632643/1e2,
    -1.0450598/1e2, -5.1920577/1e4},
    {0, 2.508355e1, 7.860106/1e2, -2.503131/1e1, 8.315270/1e2, -1.228034/1e2, 9.804036/1e4,
    -4.413030/1e5, 1.057734/1e6, -1.052755/1e8}, 
    {-1.318058e2, 4.830222e1, -1.646031, 5.464731/1e2, -9.650715/1e4,8.802193/1e6, -3.110810/1e8}
};

static const float Var_TtoV_K[3][11] =
{
    {0, 3.9450128025/1e2, 2.3622373598/1e5, -3.2858906784/1e7, -4.9904828777/1e9, -6.7509059173/1e11, 
    -5.7410327428/1e13, -3.1088872894/1e15,-1.0451609365/1e17, -1.9889266878/1e20, -1.6322697486/1e23},
    {-1.7600413686/1e2, 3.8921204975/1e2, 1.8558770032/1e5, -9.9457592874/1e8,3.1840945719/1e10, 
    -5.6072844889/1e13, 5.6075059059/1e16,-3.2020720003/1e19, 9.7151147152/1e23, -1.2104721275/1e26},
    {1.185976/1e1,-1.183432/1e4}
};

static const float Var_VtoT_S[4][10]=
{
    {0, 1.8494946e2, -8.0050406e1, 1.0223743e2, -1.5224859e2, 1.8882134e2,
    -1.5908594e2, 8.2302788e1, -2.3418194e1, 2.7978626},
    {1.2915072e1, 1.4662989e2, -1.5347134e1, 3.1459460, -4.1632578/1e1,
    3.1879638/1e2, -1.2916375/1e3, 2.1834751/1e5, -1.4473795/1e7,
    8.2112721/1e9},
    {-8.0878011e1, 1.6215731e2, -8.5368695, 4.7196870/1e1, -1.4416937/1e2,
    2.0816189/1e4},
    {5.3338751e4, -1.2358923e4, 1.0926576e3, -4.2656937e1, 6.2472054/1e1}
};

static const float Var_TtoV_S[3][9] =
{
    {0, 5.40313309/1e3, 1.25934290/1e5, -2.32477969/1e8, 3.22028823/1e11,
    -3.31465196/1e14, 2.55744252/1e17, -1.25068871/1e20, 2.71443176/1e24},
    {1.32900444, 3.34509311/1e3, 6.54805193/1e6, -1.64856259/1e9,
    1.29989605/1e14, 0, 0, 0, 0},
    {1.46628233e2, -2.58430517/1e1, 1.63693575/1e4, -3.30439047/1e8,
    -9.43223691/1e15, 0, 0, 0, 0}
};

static const float Var_VtoT_R[4][11]=
{
    {0, 1.8891380e2, -9.3835290e1, 1.3068619e2, -2.2703580e2, 3.5145659e2,
    -3.8953900e2, 2.8239471e2, -1.2607281e2, 3.1353611e1, -3.3187769},
    {1.3345845e1, 1.4726446e2, -1.8440248e1, 4.0311297, -6.2494284/1e1,
    6.4684120/1e2, -4.4587504/1e3, 1.9947101/1e4, -5.3134018/1e6,
    6.4819762/1e8},
    {-8.1995994e1, 1.5539620e2, -8.3421977, 4.2794335/1e1,
    -1.1915780/1e2, 1.4922901/1e4},
    {3.4061778e4, -7.0237292e3, 5.5829038e2, -1.9523946e1, 2.5607402/1e1}
};

static const float Var_TtoV_R[3][10]=
{
    {0, 5.28961730/1e3, 1.39166590/1e5, -2.38855693/1e8, 3.56916001/1e11,
    -4.62347666/1e14, 5.00777441/1e17, -3.73105886/1e20, 1.57716482/1e23,
    -2.81038625/1e27},
    {2.95157925, -2.52061251/1e3, 1.59564502/1e5, -7.64085948/1e9,
    2.05305291/1e12, -2.93359668/1e16},
    {1.52232118e2, -2.68819889/1e1, 1.71280280/1e4, -3.45895706/1e8,
    -9.34633971/1e15}
};

static const float Var_VtoT_B[2][9] =
{
    {9.8423321e1, 6.9971500e2, -8.4765304e2, 1.0052644e3, -8.3345952e2,
    4.5508542e2, -1.5523037e2, 2.9886750e1, -2.4742860},
    {2.1315071e2, 2.8510504e2, -5.2742887e1, 9.9160804, -1.2965303,
    1.1195870/1e1, -6.0625199/1e3, 1.8661696/1e4, -2.4878585/1e6}
};

static const float Var_TtoV_B[2][9] =
{
    {0, -2.46508183/1e4, 5.90404212/1e6, -1.32579316/1e9, 1.56682919/1e12,
    -1.69445292/1e15, 6.29903471/1e19} ,
    {-3.89381686, 2.85717475/1e2, -8.48851048/1e5, 1.57852802/1e7,
    -1.68353449/1e10, 1.11097940/1e13, -4.45154310/1e17, 9.89756408/1e21,
    -9.37913303/1e25}
};

static const float Var_VtoT_N[3][10]=
{
    {0, 3.8436847e1, 1.1010485, 5.2229312, 7.2060525, 5.8488586,
    2.7754916, 7.7075166/1e1, 1.1582665/1e1, 7.3138868/1e3},
    {0, 3.86896e1, -1.08267, 4.70205/1e2, -2.12169/1e6, -1.17272/1e4,
    5.39280/1e6, -7.98156/1e8},
    {1.972485e1, 3.300943e1, -3.915159/1e1, 9.855391/1e3,
    -1.274371/1e4, 7.767022/1e7}
};

static const float Var_TtoV_N[2][11]=
{
    {0, 2.61591060/1e2, 1.09574842/1e5, -9.38411116/1e8, -4.64120398/1e11,
    -2.63033577/1e12, -2.26534380/1e14, -7.60893008/1e17, -9.34196678/1e20},
    {0, 2.59293946/1e2, 1.57101419/1e5, 4.38256272/1e8, -2.52611698/1e10,
    6.43118193/1e13, -1.00634715/1e15, 9.97453390/1e19, -6.08632456/1e22,
    2.08492293/1e25, -3.06821962/1e29}
};

static const float Var_VtoT_E[2][10]=
{
    {0, 1.6977288e1, -4.3514970/1e1, -1.5859697/1e1, -9.2502871/1e2,
    -2.6084314/1e2, -4.1360199/1e3, -3.4034030/1e4, -1.1564890/1e5},
    {0, 1.7057035e1, -2.3301759/1e1, 6.5435585/1e3, -7.3562749/1e5,
    -1.7896001/1e6, 8.4036165/1e8, -1.3735879/1e9,
    1.0629823/1e11, -3.2447087/1e14}
};

static const float Var_TtoV_E[2][14]=
{
    {0, 5.8665508708/1e2, 4.5410977124/1e5, -7.7998048686/1e7,
    -2.5800160843/1e8, -5.9452583057/1e10, -9.3214058667/1e12,
    -1.0287605534/1e13, -8.0370123621/1e16, -4.3979497391/1e18,
    -1.6414776355/1e20, -3.9673619516/1e23, -5.5827328721/1e26,
    -3.4657842013/1e29},
    {0, 5.8665508710/1e2, 4.5032275582/1e5, 2.8908407212/1e8,
    -3.3056896652/1e10, 6.50244033/1e13, -1.9197495594/1e16, -1.2536600497/1e18,
    2.1489217569/1e21, -1.4388041782/1e24, 3.5960899481/1e28}
};

static const float Var_VtoT_J[3][9] =
{
    {0, 1.9528268e1, -1.2286185, -1.0752178, -5.9086933/1e1,
    -1.7256713/1e1, -2.8131513/1e2, -2.3963370/1e3, -8.3823321/1e5},
    {0, 1.978425e1, -2.001204/1e1, 1.036969/1e2, -2.549687/1e4,
    3.585153/1e6, -5.344285/1e8, 5.099890/1e10},
    {-3.1135819e3, 3.0054368e2, -9.9477323, 1.7027663/1e1,
    -1.4303347/1e3, 4.7388608/1e6}
};

static const float Var_TtoV_J[2][9] =
{
    {0, 5.03811878/1e2, 3.04758370/1e5, -8.56810657/1e8, 1.32281953/1e10,
    -1.70529583/1e13, 2.09480907/1e16, -1.25383953/1e19, 1.56317256/1e23},
    {2.96456257e2, -1.49761278, 3.17871039/1e3, -3.18476867/1e6,
    1.57208190/1e9, -3.06913691/1e13}
};

static const float Var_VtoT_T[2][8] =
{
    {0, 2.5949192e1, -2.1316967/1e1, 7.9018692/1e1, 4.2527777/1e1,
    1.3304473/1e1, 2.0241446/1e2, 1.2668171/1e3},
    {0, 2.592800e1, -7.602961/1e1, 4.637791/1e2, -2.165394/1e3,
    6.048144/1e5, -7.293422/1e7}
};

static const float Var_TtoV_T[2][15]=
{
    {0, 3.8748106364/1e2, 4.4194434347/1e5, 1.1844323105/1e7,
    2.0032973554/1e8, 9.0138019559/1e10, 2.2651156593/1e11,
    3.6071154205/1e13, 3.8493939883/1e15, 2.8213521925/1e17,
    1.4251594779/1e19, 4.8768662286/1e22, 1.0795539270/1e24,
    1.3945027062/1e27, 7.9795153927/1e31},
    {0, 3.8748106364/1e2, 3.3292227880/1e5, 2.0618243404/1e7,
    -2.1882256846/1e9, 1.0996880928/1e11, -3.0815758772/1e14,
    4.5479135290/1e17, -2.7512901673/1e20}
};

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��K
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float K_VtoT(float mv)
{
	uint8_t i = 0;    
	float value = 0;

	if(mv >= -6.478 && mv < 0)
	{
		value = Var_VtoT_K[0][8];
		for(i = 8; i >0; i--)
			value = mv * value + Var_VtoT_K[0][i-1];
	}
	else if(mv >= 0 && mv < 20.644)
	{
		value = Var_VtoT_K[1][9];
		for(i = 9; i >0; i--)
			value = mv * value + Var_VtoT_K[1][i-1];
	}
	else if(mv >= 20.644 && mv <= 54.900)
	{
		value = Var_VtoT_K[2][6];
		for(i = 6; i >0; i--)
			value = mv * value + Var_VtoT_K[2][i-1];
	}

	return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��K
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float K_TtoV(float temp)
{
	uint8_t i = 0;
	float value = 0;

	if((temp < -270) || (temp > 1370.0))
	{
		return ERR;
	}
	else if(temp < 0)
	{
		value = Var_TtoV_K[0][10];
		for(i = 10; i > 0; i--)
			value = temp * value + Var_TtoV_K[0][i-1];
		value += Var_TtoV_K[2][0] * exp(Var_TtoV_K[2][1]*(temp - 126.9686)*(temp - 126.9686)); 
	}
	else
	{
		value = Var_TtoV_K[1][9];
		for(i = 9; i > 0; i--)
			value = temp * value + Var_TtoV_K[1][i-1];
		value += Var_TtoV_K[2][0] * exp(Var_TtoV_K[2][1]*(temp- 126.9686)*(temp - 126.9686)); 
	}

	return value;
}

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��S
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float S_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= -0.245 && mv < 1.874)
    {
        value = Var_VtoT_S[0][9];
        for(i = 9; i > 0; i--)
            value = mv * value + Var_VtoT_S[0][i-1];
    }
    else if(mv >= 1.874 && mv < 11.950)
    {
        value = Var_VtoT_S[1][9];
        for(i = 9; i > 0; i--)
            value = mv * value + Var_VtoT_S[1][i-1];
    }
    else if(mv >= 11.950 && mv < 17.536)
    {
        value = Var_VtoT_S[2][5];
        for(i = 5; i > 0; i--)
            value = mv * value + Var_VtoT_S[2][i-1];
    }
    else
    {
        value = Var_VtoT_S[3][4];
        for(i = 4; i > 0; i--)
            value = mv * value + Var_VtoT_S[3][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��S
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float S_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < -50) || (temp > 1760.0))
    {
        return ERR;
    }
    else if((temp >= -50) && (temp < 1064.18))
    {
        value = Var_TtoV_S[0][8];
        for(i = 8; i > 0; i--)
            value = temp * value + Var_TtoV_S[0][i-1];
    }
    else if(temp >= 1064.18 && temp < 1664.5)
    {
        value = Var_TtoV_S[1][4];
        for(i = 4; i > 0; i--)
            value = temp * value + Var_TtoV_S[1][i-1];
    }
    else
    {
        value = Var_TtoV_S[2][4];
        for(i=4; i > 0; i--)
            value=temp * value + Var_TtoV_S[2][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��R
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float R_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= -0.240 && mv < 1.923)
    {
        value = Var_VtoT_R[0][10] ;
        for(i = 10; i > 0; i--)
        value = mv * value + Var_VtoT_R[0][i-1];
    }
    else if(mv >= 1.923 && mv < 13.228)
    {
        value = Var_VtoT_R[1][9];
        for(i = 9; i > 0; i--)
            value = mv * value + Var_VtoT_R[1][i-1];
    }
    else if(mv >= 11.361 && mv < 19.739)
    {
        value = Var_VtoT_R[2][5];
        for(i = 5; i > 0; i--)
        value = mv * value + Var_VtoT_R[2][i-1];
    }
    else
    {
        value = Var_VtoT_R[3][4];
        for(i = 4; i > 0; i--)
        value = mv * value + Var_VtoT_R[3][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��R
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float R_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < -50) || (temp > 1760.0))
    {
        return ERR;
    }
    else if(temp >= -50 && temp < 1064.18)
    {
        value = Var_TtoV_R[0][9];
        for(i = 9; i > 0; i--)
            value = temp * value + Var_TtoV_R[0][i-1];
    }
    else if(temp >= 1064.18 && temp < 1664.5)
    {
        value = Var_TtoV_R[1][5];
        for(i = 5; i > 0; i--)
            value = temp * value + Var_TtoV_R[1][i-1];
    }
    else
    {
        value = Var_TtoV_R[2][4];
        for(i = 4; i > 0; i--)
            value = temp * value + Var_TtoV_R[2][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��B
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float B_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= 0.033 && mv < 1.978)
    {
        value = Var_VtoT_B[0][8];
        for(i = 8; i > 0; i--)
            value = mv * value + Var_VtoT_B[0][i-1];
    }
    else
    {
        value = Var_VtoT_B[1][8];
        for(i = 8; i > 0; i--)
            value = mv * value + Var_VtoT_B[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��B
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float B_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < 0) || (temp > 1820.0))
    {
        return ERR;
    }
    else if(temp < 100)
    {
        value = 0;
    }
    else if(temp >= 100 && temp < 630.615)
    {
        value = Var_TtoV_B[0][6];
        for(i = 6; i > 0; i--)
            value = temp * value + Var_TtoV_B[0][i-1];
    }
    else
    {
        value = Var_TtoV_B[1][8];
        for(i = 8; i > 0; i--)
            value = temp * value + Var_TtoV_B[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��N
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float N_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= -4.365 && mv < 0)
    {
        value = Var_VtoT_N[0][9];
        for(i = 9; i >0; i--)
        value = mv * value + Var_VtoT_N[0][i-1];
    }
    else if(mv >= 0 && mv < 20.613)
    {
        value = Var_VtoT_N[1][7];
        for(i = 7; i >0; i--)
        value = mv * value + Var_VtoT_N[1][i-1];
    }
    else if(mv >= 20.613 && mv <= 47.533)
    {
        value = Var_VtoT_N[2][5];
        for(i = 5; i > 0; i--)
        value = mv * value + Var_VtoT_N[2][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��N
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float N_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < -270) || (temp > 1300.0))
    {
        return ERR;
    }
    else if(temp < 0)
    {
        value = Var_TtoV_N[0][8];
        for(i = 8; i > 0; i--)
            value = temp * value + Var_TtoV_N[0][i-1];
    }
    else
    {
        value = Var_TtoV_N[1][10];
        for(i = 10; i > 0; i--)
            value = temp * value + Var_TtoV_N[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��E
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float E_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= -9.845 && mv < 0)
    {
        value = Var_VtoT_E[0][8];
        for(i = 8; i > 0; i--)
            value = mv * value + Var_VtoT_E[0][i-1];
    }
    else if(mv >= 0 && mv <= 76.393)
    {
        value = Var_VtoT_E[1][9];
        for(i = 9; i > 0; i--)
            value = mv * value + Var_VtoT_E[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��E
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float E_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < -270) || (temp > 1000.0))
    {
        return ERR;
    }
    else if(temp < 0)
    {
        value = Var_TtoV_E[0][13];
        for(i = 13; i > 0; i--)
            value = temp * value + Var_TtoV_E[0][i-1];
    }
    else
    {
        value = Var_TtoV_E[1][10];
        for(i = 10; i > 0; i--)
            value = temp * value + Var_TtoV_E[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ���ѹת�¶�
* ��  �Σ���ѹֵ(mv)
* ����ֵ���¶�ֵ
* ��  ע��J
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float J_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= -8.105 && mv < 0)
    {
        value = Var_VtoT_J[0][8];
        for(i = 8; i > 0; i--)
        value = mv * value + Var_VtoT_J[0][i-1];
    }
    else if(mv >= 0 && mv < 42.919)
    {
        value = Var_VtoT_J[1][7];
        for(i = 7; i > 0; i--)
        value = mv * value + Var_VtoT_J[1][i-1];
    }
    else if(mv>=42.919 && mv<=69.563)
    {
        value = Var_VtoT_J[2][5];
        for(i = 5; i > 0; i--)
        value = mv * value + Var_VtoT_J[2][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��J
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float J_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < -210) || (temp > 1200.0))
    {
        return ERR;
    }
    else if(temp >= -210 && temp < 760)
    {
        value = Var_TtoV_J[0][8];
        for(i = 8; i > 0; i--)
            value = temp * value + Var_TtoV_J[0][i-1];
    }
    else
    {
        value = Var_TtoV_J[1][5];
        for(i = 5; i > 0; i--)
            value = temp * value + Var_TtoV_J[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��T
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float T_VtoT(float mv)
{
    uint8_t i = 0;
    float value = 0;
    
    if(mv >= -6.268 && mv < 0)
    {
        value = Var_VtoT_T[0][7];
        for(i = 7; i >0; i--)
            value = mv * value + Var_VtoT_T[0][i-1];
    }
    else if(mv >= 0 && mv <= 20.882)
    {
        value = Var_VtoT_T[1][6];
        for(i = 6; i >0; i--)
            value = mv * value + Var_VtoT_T[1][i-1];
    }
    
    return value;
}

/*
*********************************************************************************************************
* ��  �ܣ��¶�ת��ѹ
* ��  �Σ��¶�ֵ
* ����ֵ����ѹֵ(mv)
* ��  ע��T
* ��  �ߣ�LG
* ��  �ڣ�2020/11/17
*********************************************************************************************************
*/

float T_TtoV(float temp)
{
    uint8_t i = 0;
    float value = 0;
    
    if((temp < -270) || (temp > 400.0))
    {
        return ERR;
    }
    else if(temp < 0)
    {
        value = Var_TtoV_T[0][14];
        for(i = 14; i > 0; i--)
            value = temp * value + Var_TtoV_T[0][i-1];
    }
    else
    {
        value = Var_TtoV_T[1][8];
        for(i = 8; i > 0; i--)
            value = temp * value + Var_TtoV_T[1][i-1];
    }
	
    return value;
}


