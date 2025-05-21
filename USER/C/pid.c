#include "pid.h"
#include "servo.h"

//定义全局变量

_pid pid_positionX, pid_positionY, pid_speedX, pid_speedY;



/**
  * @brief  PID参数初始化
	*	@note 	无
  * @retval 无
  */
void PID_param_init(void)
{
		/* X位置相关初始化参数 */
    pid_positionX.target_val=0.0;				
    pid_positionX.actual_val=0.0;
    pid_positionX.err=0.0;
    pid_positionX.err_last=0.0;
    pid_positionX.integral=0.0;
  
		pid_positionX.Kp = 0.0;
		pid_positionX.Ki = 0.0;
		pid_positionX.Kd = 0.0;
	
		/* Y位置相关初始化参数 */
    pid_positionY.target_val=0.0;				
    pid_positionY.actual_val=0.0;
    pid_positionY.err=0.0;
    pid_positionY.err_last=0.0;
    pid_positionY.integral=0.0;
  
		pid_positionY.Kp = 0.0;
		pid_positionY.Ki = 0.0;
		pid_positionY.Kd = 0.0;

  
  	/* X速度相关初始化参数 */
    pid_speedX.target_val=0.0;				
    pid_speedX.actual_val=0.0;
    pid_speedX.err=0.0;
    pid_speedX.err_last=0.0;
    pid_speedX.integral=0.0;
  
		pid_speedX.Kp = 0.0;
		pid_speedX.Ki = 0.0;
		pid_speedX.Kd = 0.0;
		
		/* Y速度相关初始化参数 */
    pid_speedY.target_val=0.0;				
    pid_speedY.actual_val=0.0;
    pid_speedY.err=0.0;
    pid_speedY.err_last=0.0;
    pid_speedY.integral=0.0;
  
		pid_speedY.Kp = 0.0;
		pid_speedY.Ki = 0.0;
		pid_speedY.Kd = 0.0;
		
} 

/**
  * @brief  设置目标值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
	*	@note 	无
  * @retval 无
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // 设置比例系数 P
	pid->Ki = i;    // 设置积分系数 I
	pid->Kd = d;    // 设置微分系数 D
}

/*****************************下面的数值全部要经过计算后重新改*************************/

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
float location_pid_realize(_pid *pid, float actual_val)
{
		/*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;
  
	
	/*************限定闭环死区、积分分离里面设置的比较值要搞清楚怎么设才合理，开始调试的时候不能够太小，否则调参看不见效果。也不能太大，严重时会破坏机械结构*****/
    /* 限定闭环死区 */   //这个直接实操看那个圈对应数值大小
    if((pid->err >= - 2) && (pid->err <= 2))
    {
        pid->err = 0;
        pid->integral = 0;
    }
    
    /* 积分分离，偏差较大时去掉积分作用 */  
    if (pid->err > -10 && pid->err < 10)
    {
        pid->integral += pid->err;    // 误差累积
        
        /* 限定积分范围，防止积分饱和 */
        if (pid->integral >10) pid->integral =10;
        else if (pid->integral < 10)  pid->integral = -10;
    }

		/*PID算法实现*/
    pid->actual_val = pid->Kp * pid->err + 
                      pid->Ki * pid->integral + 
                      pid->Kd * (pid->err - pid->err_last);
  
		/*误差传递*/
    pid->err_last = pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}

/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
		/*计算目标值与实际值的误差*/
    pid->err = pid->target_val - actual_val;

	
	//这下面的比较值设的越大，那么就表示速度环控制精度越低。  
	//加入把数值为1的偏差都忽略，那么1s的时间小球位移个10个单位坐标都会当作没误差
    if((pid->err<0.5f ) && (pid->err>-0.5f))   //这个数值的设置也要弄懂，要设置合理  
        pid->err = 0.0f;   

    pid->integral += pid->err;    // 误差累积

		/*PID算法实现*/
    pid->actual_val = pid->Kp * pid->err + 
                      pid->Ki * pid->integral + 
                      pid->Kd * (pid->err - pid->err_last);
  
		/*误差传递*/
    pid->err_last = pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}

/************************************ END OF FIER *******************************************************/
















