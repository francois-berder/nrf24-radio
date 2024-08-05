/* 
 * File:   softspi.h
 * Author: francois
 *
 * Created on September 15, 2023, 4:52 PM
 */

#ifndef SOFTSPI_H
#define	SOFTSPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    void SOFTSPI_ExchangeBlock(void *block, size_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* SOFTSPI_H */

