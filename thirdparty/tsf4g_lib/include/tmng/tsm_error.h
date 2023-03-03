#ifndef TSM_ERROR_H_

/*!
 * \file tsm_error.h
 * \brief tsm error api
 * \version 1.0
 * date 2013-03-05
 */

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \fn      void tsm_error_clear(void);
 * \brief   把tsm错误堆栈清空
 * \param   空
 * \return  空
 */
void tsm_error_clear(void);

/*!
 * \fn      void tsm_error_dump(void);
 * \brief   把tsm错误堆栈输出至屏幕
 * \param   空
 * \return  空
 */
void tsm_error_dump(void);

/*!
 * \fn      void tsm_error_dump(void);
 * \brief   把tsm错误堆栈输出至字符缓冲区
 * \param
 *          \li a_pszErr    记录错误信息缓冲区地址
 *          \li a_iSize     记录错误信息的缓冲区空间大小
 * \return  成功返回0，否则返回错误码
 *          \li -1          参数错误，a_pszErr为空或a_iSize为0
 *          \li -2          获取全局错误堆栈变量失败
 *          \li -3          错误信息缓冲区空间不够
 */
int tsm_error_get(char *a_pszErr, size_t a_iSize);

#ifdef __cplusplus
}
#endif

#define TSM_ERROR_H_
#endif
