// TestsAnalyzСonfApp implements some useful functions 
// for implementing A-SMGCS (Advanced Surface Movement Guidance and Control System).
// author: Roman Ermakov
// e-mail: romul1508@gmail.com
// sinc 16.11.2021
// version: 1.0.1
// AnalyzerСonflict Copyright 2021 Roman Ermakov
// All rights reserved

#include "Pulkovo.h"
#include "multi_attr.h"

void WgsToLocPlkv_zone_1_square_11(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square11_Y.fuz" );

	temp_lat = Lat - 59.8;			// уберем неинформативную часть
	temp_lat = temp_lat / 0.001871;		// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	
	//printf("%4.4f\n", complAss);
	//----------------------------------
	//----------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square11_X.fuz" );

	temp_lon = Lon - 30.3;					// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;		// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_11(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lon;

	temp_y = Y * 0.005;		//   делим на 200
	*pLat = temp_y * 0.001871 + 59.8;

	fuzzy_block_t* blk_1;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square11_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_1 );
	
	temp_lon = blk_1->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.3);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
}
//-------------------------------------------------------------
void WgsToLocPlkv_zone_1_square_25(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square25_Y.fuz" );

	temp_lat = Lat - 59.8;							// уберем неинформативную часть
	temp_lat = temp_lat / 0.0055995;		// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square25_X.fuz" );

	temp_lon = Lon - 30.27;					// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;		// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_25(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square25_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0055995;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone1Square25_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.27);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void WgsToLocPlkv_zone_1_square_26(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square26_Y.fuz" );

	temp_lat = Lat - 59.8;					// уберем неинформативную часть
	temp_lat = temp_lat / 0.005567;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square26_X.fuz" );

	temp_lon = Lon - 30.27;					// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_26(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square26_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.005567;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone1Square26_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.27);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void WgsToLocPlkv_zone_1_square_27(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square27_Y.fuz" );

	temp_lat = Lat - 59.8;					// уберем неинформативную часть
	temp_lat = temp_lat / 0.005772;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square27_X.fuz" );

	temp_lon = Lon - 30.2;					// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_27(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square27_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.005772;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone1Square27_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void WgsToLocPlkv_zone_1_square_34(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square34_Y.fuz" );

	temp_lat = Lat - 59.8;							// уберем неинформативную часть
	temp_lat = temp_lat / 0.007341;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square34_X.fuz" );

	temp_lon = Lon - 30.26;					// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 200.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_34(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square34_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.007341;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone1Square34_Lon.fuz" );

	temp_x = X * 0.005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.26);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void WgsToLocPlkv_zone_1_square_35(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square35_Y.fuz" );

	temp_lat = Lat - 59.8;							// уберем неинформативную часть
	temp_lat = temp_lat / 0.0073775;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square35_X.fuz" );

	temp_lon = Lon - 30.2;					// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_35(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square35_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0073775;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone1Square35_Lon.fuz" );

	temp_x = X * 0.005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void WgsToLocPlkv_zone_1_square_36(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone1Square36_Y.fuz" );

	temp_lat = Lat - 59.8;							// уберем неинформативную часть
	temp_lat = temp_lat / 0.007346;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone1Square36_X.fuz" );

	temp_lon = Lon - 30.2;					// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
void PlkvLocToWgs_zone_1_square_36(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone1Square36_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.007346;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone1Square36_Lon.fuz" );

	temp_x = X * 0.005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//------------------------------------------------------------
int WgsToLocPlkv_zone_1(double Lat, double Lon, double *pX, double *pY)
{
	int calc_completed = true;
	
	if(  Lat > 59.8 &&  Lat < 59.801871 &&  Lon > 30.3008345 && Lon < 30.304236)
		WgsToLocPlkv_zone_1_square_11(Lat, Lon, pX, pY);
	else if( Lat > 59.8037815 && Lat < 59.8055995 && Lon > 30.2719695 && Lon < 30.275573)
		WgsToLocPlkv_zone_1_square_25(Lat, Lon, pX, pY);
	else if( Lat > 59.8038275 && Lat < 59.805567 && Lon > 30.275573 && Lon < 30.2793895 )
		WgsToLocPlkv_zone_1_square_26(Lat, Lon, pX, pY);
	else if( Lat > 59.8038355 && Lat < 59.805772 && Lon > 30.2793895 && Lon < 30.282872 )
		WgsToLocPlkv_zone_1_square_27(Lat, Lon, pX, pY);
	else if( Lat > 59.805684 && Lat < 59.807341 && Lon > 30.264675 && Lon < 30.2680555 )
		WgsToLocPlkv_zone_1_square_34(Lat, Lon, pX, pY);
	else if( Lat > 59.8056455 && Lat < 59.8073775 && Lon > 30.2680555 && Lon < 30.2720025 )
		WgsToLocPlkv_zone_1_square_35(Lat, Lon, pX, pY);
	else if( Lat > 59.8055995 && Lat < 59.807346 && Lon > 30.2680555 && Lon < 30.2755935 )
		WgsToLocPlkv_zone_1_square_36(Lat, Lon, pX, pY);
	else
	{
		char str[90];
		sprintf(str, "In Zone 1, the coordinates could not be determined, Lat = %8.6f, Lon = %8.6f", Lat, Lon); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
void WgsToLocPlkv_zone_2_square_1(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone2Square1_Y.fuz" );

	temp_lat = Lat - 59.8;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.0020515;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 200.0;		  // возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone2Square1_X.fuz" );

	temp_lon = Lon - 30.26;					// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 200.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_2_square_1(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone2Square1_Lat.fuz" );

	temp_y = Y * 0.005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0020515;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone2Square1_Lon.fuz" );

	temp_x = X * 0.005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.26);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_2_square_2(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone2Square2_Y.fuz" );

	temp_lat = Lat - 59.8;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.0021205;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 200.0;		  // возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone2Square2_X.fuz" );

	temp_lon = Lon - 30.2;					// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_2_square_2(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone2Square2_Lat.fuz" );

	temp_y = Y * 0.005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0021205;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone2Square2_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_2_square_14(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone2Square14_Y.fuz" );

	temp_lat = Lat - 59.8;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.0020225;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 200.0;		  // возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone2Square14_X.fuz" );

	temp_lon = Lon - 30.21;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_2_square_14(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone2Square14_Lat.fuz" );

	temp_y = Y * 0.005;	// фаззифицируем Y (делим на 200)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0020225;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone2Square14_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.21);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_2_square_76(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone2Square76_Y.fuz" );

	temp_lat = Lat - 59.8;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.0109165;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = blk_1->outputs[0] * 2000.0;		  // возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone2Square76_X.fuz" );

	temp_lon = Lon - 30.2;						// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_2_square_76(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone2Square76_Lat.fuz" );

	temp_y = Y * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0109165;
	*pLat = (temp_lat + 59.8);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone2Square76_Lon.fuz" );

	temp_x = X * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
int WgsToLocPlkv_zone_2(double Lat, double Lon, double *pX, double *pY)
{
	int calc_completed = true;
	
	if(  Lat > 59.8001425 &&  Lat < 59.8020515 &&  Lon > 30.261242 && Lon < 30.264759)
		WgsToLocPlkv_zone_2_square_1(Lat, Lon, pX, pY);
	else if( Lat > 59.8002385 && Lat < 59.8021205 && Lon > 30.257508 && Lon < 30.261242 )
		WgsToLocPlkv_zone_2_square_2(Lat, Lon, pX, pY);
	else if( Lat > 59.800374 && Lat < 59.8020225 && Lon > 30.2156445 && Lon < 30.2191635 )
		WgsToLocPlkv_zone_2_square_14(Lat, Lon, pX, pY);
	else if( Lat > 59.809198 && Lat < 59.8109165 && Lon > 30.2440705 && Lon < 30.264759)
		WgsToLocPlkv_zone_2_square_76(Lat, Lon, pX, pY);
	else
	{
		char str[90];
		sprintf(str, "In Zone 2, the coordinates could not be determined, Lat = %8.6f, Lon = %8.6f", Lat, Lon); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_1(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square1_Y.fuz" );

	temp_lat = Lat - 59.8;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.8001425;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 200.0;		  // возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square1_X.fuz" );

	temp_lon = Lon - 30.26;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 200.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_1(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square1_Lat.fuz" );

	temp_y = abs(Y) * 0.005;	// фаззифицируем Y (делим на 200)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.8001425;
	*pLat = (temp_lat + 59);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square1_Lon.fuz" );

	temp_x = abs(X) * 0.005;						// фаззифицируем X (делим на 200)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.26);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_14(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square14_Y.fuz" );

	temp_lat = Lat - 59;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.800374;			// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 200.0;		  // возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square14_X.fuz" );

	temp_lon = Lon - 30.21;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_14(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square14_Lat.fuz" );

	temp_y = abs(Y) * 0.005;	// фаззифицируем Y (делим на 200)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.800374;
	*pLat = (temp_lat + 59);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square14_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 200)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.21);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_15(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square15_Y.fuz" );

	temp_lat = Lat - 59.79;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.0084115;				// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square15_X.fuz" );

	temp_lon = Lon - 30.26;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 200.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_15(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square15_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0084115;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square15_Lon.fuz" );

	temp_x = abs(X) * 0.005;						// фаззифицируем X (делим на 200)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.26);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_29(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square29_Y.fuz" );

	temp_lat = Lat - 59.79;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.0065075;				// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square29_X.fuz" );

	temp_lon = Lon - 30.26;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 200.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_29(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square29_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0065075;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square29_Lon.fuz" );

	temp_x = abs(X) * 0.005;						// фаззифицируем X (делим на 200)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.26);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_31(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square31_Y.fuz" );

	temp_lat = Lat - 59.79;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.006408;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square31_X.fuz" );

	temp_lon = Lon - 30.25;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_31(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square31_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.006408;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square31_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 200)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.25);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_32(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square32_Y.fuz" );

	temp_lat = Lat - 59.79;								// уберем неинформативную часть
	temp_lat = temp_lat / 0.006462;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square32_X.fuz" );

	temp_lon = Lon - 30.25;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_32(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square32_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.006462;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square32_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.25);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_33(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square33_Y.fuz" );

	temp_lat = Lat - 59.79;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.0064945;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square33_X.fuz" );

	temp_lon = Lon - 30.2;						// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_33(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square33_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0064945;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square33_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_45(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square45_Y.fuz" );

	temp_lat = Lat - 59.79;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.0046885;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square45_X.fuz" );

	temp_lon = Lon - 30.25;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_45(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square45_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0046885;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square45_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.25);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_3_square_46(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone3Square46_Y.fuz" );

	temp_lat = Lat - 59.79;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.0047105;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone3Square46_X.fuz" );

	temp_lon = Lon - 30.25;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = -blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_3_square_46(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone3Square46_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0047105;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone3Square46_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.25);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
int WgsToLocPlkv_zone_3(double Lat, double Lon, double *pX, double *pY)
{
	int calc_completed = true;
	
	if(  Lat > 59.7984115 &&  Lat < 59.8001425 &&  Lon > 30.2611475 && Lon < 30.264817)
		WgsToLocPlkv_zone_3_square_1(Lat, Lon, pX, pY);
	else if( Lat > 59.798506 && Lat < 59.800374 && Lon > 30.2152225 && Lon < 30.219031)
		WgsToLocPlkv_zone_3_square_14(Lat, Lon, pX, pY);
	else if( Lat > 59.7965075 && Lat < 59.7984115 && Lon > 30.260965 && Lon < 30.2647435 )
		WgsToLocPlkv_zone_3_square_15(Lat, Lon, pX, pY);
	else if( Lat > 59.7945615 && Lat < 59.7965075 && Lon > 30.260945 && Lon < 30.2647245 )
		WgsToLocPlkv_zone_3_square_29(Lat, Lon, pX, pY);
	else if( Lat > 59.7946885 && Lat < 59.796408 && Lon > 30.2541535 && Lon < 30.257329 )
		WgsToLocPlkv_zone_3_square_31(Lat, Lon, pX, pY);
	else if( Lat > 59.7947105 && Lat < 59.796462 && Lon > 30.250806 && Lon < 30.2541535 )
		WgsToLocPlkv_zone_3_square_32(Lat, Lon, pX, pY);
	else if( Lat > 59.7947325 && Lat < 59.7964945 && Lon > 30.24733 && Lon < 30.250806 )
		WgsToLocPlkv_zone_3_square_33(Lat, Lon, pX, pY);
	else if( Lat > 59.7929045 && Lat < 59.7946885 && Lon > 30.2541105 && Lon < 30.257286 )
		WgsToLocPlkv_zone_3_square_45(Lat, Lon, pX, pY);
	else if( Lat > 59.7928075 && Lat < 59.7947105 && Lon >30.2506775 && Lon < 30.2541105)
		WgsToLocPlkv_zone_3_square_46(Lat, Lon, pX, pY);
	else
	{
		char str[80];
		sprintf(str, "In Zone 3, the coordinates could not be determined, Lat = %8.6f, Lon = %8.6f", Lat, Lon); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
void WgsToLocPlkv_zone_4_square_29(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone4Square29_Y.fuz" );

	temp_lat = Lat - 59.79;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.0027555;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone4Square29_X.fuz" );

	temp_lon = Lon - 30.2;						// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_4_square_29(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone4Square29_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0027555;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone4Square29_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_4_square_30(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone4Square30_Y.fuz" );

	temp_lat = Lat - 59.79;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.0028095;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone4Square30_X.fuz" );

	temp_lon = Lon - 30.28;						// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_4_square_30(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone4Square30_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.0028095;
	*pLat = (temp_lat + 59.79);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone4Square30_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.28);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_4_square_35(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone4Square35_Y.fuz" );

	temp_lat = Lat - 59.7;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.090809;					// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone4Square35_X.fuz" );

	temp_lon = Lon - 30.2;						// уберем неинформативную часть
	temp_lon = temp_lon * 10.0;			// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_4_square_35(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone4Square35_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.090809;
	*pLat = (temp_lat + 59.7);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone4Square35_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.1;
	*pLon = (temp_lon + 30.2);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void WgsToLocPlkv_zone_4_square_36(double Lat, double Lon, double *pX, double *pY)
{
	int i;
	double temp_lat, temp_lon;
	
	temp_lat = Lat;
	temp_lon = Lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
		
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "WgsToLocPlkvZone4Square36_Y.fuz" );

	temp_lat = Lat - 59.7;									// уберем неинформативную часть
	temp_lat = temp_lat / 0.090798;							// нормализуем остаток

	//------------------
	blk_1->inputs[0] = (float)temp_lat;
	
	contr_fuzzy( blk_1 );
	*pY = -blk_1->outputs[0] * 2000.0;			// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------------------
	//-------------------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "WgsToLocPlkvZone4Square36_X.fuz" );

	temp_lon = Lon - 30.28;				// уберем неинформативную часть
	temp_lon = temp_lon * 100.0;		// уменьшим количество нулей перед запятой

	//------------------
	blk_2->inputs[0] = (float)temp_lon;
	
	contr_fuzzy( blk_2 );
	*pX = blk_2->outputs[0] * 2000.0;		// возвращаем результат в метрах
	//---------------------

	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
void PlkvLocToWgs_zone_4_square_36(double X, double Y, double *pLat, double *pLon)
{
	int i;
	double temp_x, temp_y, temp_lat, temp_lon;

	fuzzy_block_t* blk_1;
	fuzzy_block_t* blk_2;
			
	blk_1  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_1, "PlkvLocToWgsZone4Square36_Lat.fuz" );

	temp_y = abs(Y) * 0.0005;	// фаззифицируем Y (делим на 2000)
	
	//------------------
	blk_1->inputs[0] = (float)temp_y;
	
	contr_fuzzy( blk_1 );
	
	temp_lat = blk_1->outputs[0] * 0.090798;
	*pLat = (temp_lat + 59.7);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_1->num_outp; ++i )
	{
    	free(blk_1->m_outputs[i]);
	}
	free(blk_1->m_outputs);
 
	for( i=0; i < blk_1->num_inp; ++i )
	{
		free(blk_1->m_inputs[i]);
	}
	free(blk_1->m_inputs);

	free(blk_1->outputs);
	free(blk_1->inputs);
	free(blk_1);
	//-------------------------
	//-------------------------
	blk_2  = (fuzzy_block_t*)calloc( 1, sizeof(fuzzy_block_t) );
	read_data( blk_2, "PlkvLocToWgsZone4Square36_Lon.fuz" );

	temp_x = abs(X) * 0.0005;						// фаззифицируем X (делим на 2000)
	
	//------------------
	blk_2->inputs[0] = (float)temp_x;
	
	contr_fuzzy( blk_2 );
	
	temp_lon = blk_2->outputs[0] * 0.01;
	*pLon = (temp_lon + 30.28);
	//---------------------
	// Освободим выделенную память
	for(i=0; i < blk_2->num_outp; ++i )
	{
    	free(blk_2->m_outputs[i]);
	}
	free(blk_2->m_outputs);
 
	for( i=0; i < blk_2->num_inp; ++i )
	{
		free(blk_2->m_inputs[i]);
	}
	free(blk_2->m_inputs);

	free(blk_2->outputs);
	free(blk_2->inputs);
	free(blk_2);
}
//-------------------------------------------
int WgsToLocPlkv_zone_4(double Lat, double Lon, double *pX, double *pY)
{
	int calc_completed = true;
	
	if(  Lat > 59.790809 &&  Lat < 59.7927555 &&  Lon > 30.278881 && Lon < 30.2827865)
		WgsToLocPlkv_zone_4_square_29(Lat, Lon, pX, pY);
	else if( Lat > 59.790798 && Lat < 59.7928095 && Lon > 30.2827865 && Lon < 30.2860265)
		WgsToLocPlkv_zone_4_square_30(Lat, Lon, pX, pY);
	else if( Lat > 59.7889375 && Lat < 59.790809 && Lon > 30.2788595 && Lon < 30.283001 )
		WgsToLocPlkv_zone_4_square_35(Lat, Lon, pX, pY);
	else if( Lat > 59.7888945 && Lat < 59.790798 && Lon > 30.283001 && Lon < 30.285844 )
		WgsToLocPlkv_zone_4_square_36(Lat, Lon, pX, pY);
	else
	{
		char str[80];
		sprintf(str, "In Zone 4, the coordinates could not be determined, Lat = %8.6f, Lon = %8.6f", Lat, Lon); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_1(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = true;
	
	if(X > 1999.8 && X < 2200.02 && Y > -0.02 && Y < 200.02  )
		PlkvLocToWgs_zone_1_square_11(X, Y, pLat, pLon);
	else if(X > 399.8 && X < 600.02 && Y > 399.8 && Y < 600.02)
		PlkvLocToWgs_zone_1_square_25(X, Y, pLat, pLon);
	else if(X > 599.8 && X < 800.02 && Y > 399.8 && Y < 600.02)
		PlkvLocToWgs_zone_1_square_26(X, Y, pLat, pLon);
	else if(X > 799.8 && X < 1000.02 && Y > 399.8 && Y < 600.02)
		PlkvLocToWgs_zone_1_square_27(X, Y, pLat, pLon);
	else if(X > -0.02 && X < 200.02 && Y > 599.8 && Y < 800.02)
		PlkvLocToWgs_zone_1_square_34(X, Y, pLat, pLon);
	else if(X > 199.8 && X < 400.02 && Y > 599.8 && Y < 800.02)
		PlkvLocToWgs_zone_1_square_35(X, Y, pLat, pLon);
	else if(X > 399.8 && X < 600.02 && Y > 599.8 && Y < 800.02)
		PlkvLocToWgs_zone_1_square_36(X, Y, pLat, pLon);
	else
	{
		char str[80];
		sprintf(str, "In Zone 1, the coordinates could not be determined, X = %8.4f, Y = %8.4f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_2(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = true;
	
	if(X > -200.02 && X < 0.02 && Y > -0.02 && Y < 200.02  )
		PlkvLocToWgs_zone_2_square_1(X, Y, pLat, pLon);
	else if(X > -400.02 && X < -199.8 && Y > -0.02 && Y < 200.02)
		PlkvLocToWgs_zone_2_square_14(X, Y, pLat, pLon);
	else if(X > -2800.02 && X < -2599.8 && Y > -0.02 && Y < 200.02)
		PlkvLocToWgs_zone_2_square_14(X, Y, pLat, pLon);
	else if(X > -1200.2 && X < -999.8 && Y > 999.8 && Y < 1200.02)
		PlkvLocToWgs_zone_2_square_76(X, Y, pLat, pLon);
	else
	{
		char str[90];
		sprintf(str, "In Zone 2, the coordinates could not be determined, X = %8.4f, Y = %8.4f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_3(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = 1;
	
	if(X > -200.02 && X < 0.02 && Y > -200.02 && Y < 0.02  )
		PlkvLocToWgs_zone_3_square_1(X, Y, pLat, pLon);
	else if(X > -2800.2 && X < -2599.8 && Y > -200.02 && Y < 0.02)
		PlkvLocToWgs_zone_3_square_14(X, Y, pLat, pLon);
	else if(X > -200.02 && X < 0.02 && Y > -400.02 && Y < -199.8)
		PlkvLocToWgs_zone_3_square_15(X, Y, pLat, pLon);
	else if(X > -200.02 && X < 0.02 && Y > -600.02 && Y < -399.8)
		PlkvLocToWgs_zone_3_square_29(X, Y, pLat, pLon);
	else if(X > -600.02 && X < -399.8 && Y > -600.02 && Y < -399.8)
		PlkvLocToWgs_zone_3_square_31(X, Y, pLat, pLon);
	else if(X > -800.02 && X < -599.8 && Y > -600.02 && Y < -399.8)
		PlkvLocToWgs_zone_3_square_32(X, Y, pLat, pLon);	
	else if(X > -1000.02 && X < -799.8 && Y > -600.02 && Y < -399.8)
		PlkvLocToWgs_zone_3_square_33(X, Y, pLat, pLon);	
	else if(X > -600.02 && X < -399.8 && Y > -800.02 && Y < -599.8)
		PlkvLocToWgs_zone_3_square_45(X, Y, pLat, pLon);
	else if(X > -800.02 && X < -599.8 && Y > -800.02 && Y < -599.8)
		PlkvLocToWgs_zone_3_square_46(X, Y, pLat, pLon);
	else
	{
		char str[90];
		sprintf(str, "In Zone 3, the coordinates could not be determined, X = %8.4f, Y = %8.4f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_4(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = 1;
	
	if(X > 799.8 && X < 1000.02 && Y > -1000.02 && Y < -799.8  )
		PlkvLocToWgs_zone_4_square_29(X, Y, pLat, pLon);
	else if(X > 999.8 && X < 1200.02 && Y > -1000.02 && Y < -799.8 )
		PlkvLocToWgs_zone_4_square_30(X, Y, pLat, pLon);
	else if(X > 799.8 && X < 1000.02 && Y > -1200.02 && Y < -999.8 )
		PlkvLocToWgs_zone_4_square_35(X, Y, pLat, pLon);
	else if(X > 999.8 && X < 1200.02 && Y > -1200.02 && Y < -999.8 )
		PlkvLocToWgs_zone_4_square_36(X, Y, pLat, pLon);
	else
	{
		char str[90];
		sprintf(str, "In Zone 4, the coordinates could not be determined, X = %8.4f, Y = %8.4f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = 0;
	}

	return calc_completed;
}
//-------------------------------------------
int FuzzyWgsToLocPlkv(double Lat, double Lon, double *pX, double *pY)
{
	int calc_completed = 1;
	if(Lat > 59.8 && Lon > 30.26465)
		calc_completed = WgsToLocPlkv_zone_1(Lat, Lon, pX, pY);
	else if(Lat > 59.80005 && Lon < 30.26466)
		calc_completed = WgsToLocPlkv_zone_2(Lat, Lon, pX, pY);
	else if (Lat < 59.800096 && Lon < 30.264818)
		calc_completed = WgsToLocPlkv_zone_3(Lat, Lon, pX, pY);
	else if (Lat < 59.800096 && Lon > 30.26465)
		calc_completed = WgsToLocPlkv_zone_4(Lat, Lon, pX, pY);

	return calc_completed;
}
//-------------------------------------------
int FuzzyPlkvLocToWgs(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = 1;
	if(X > -0.02 && Y > -0.02  )
		calc_completed = PlkvLocToWgs_zone_1(X, Y, pLat, pLon);
	else if(X < 0.02 && Y > -0.02)
		calc_completed = PlkvLocToWgs_zone_2(X, Y, pLat, pLon);
	else if(X < 0.02 && Y < 0.02)
		calc_completed = PlkvLocToWgs_zone_3(X, Y, pLat, pLon);
	else if( X > -0.02 && Y < 0.02 )
		calc_completed = PlkvLocToWgs_zone_4(X, Y, pLat, pLon);

	return calc_completed;
}
//--------------------------------
void WgsToLocPr(double dLat, double dLon, double *pX, double *pY)
{
	// Перевод географических координат (широты и долготы) в прямоугольные
	// координаты проекции Гаусса-Крюгера 
	
	// Географические координаты точки (в градусах)
	// double dLon = 37.618;
	// double dLat = 55.752;

	// Номер зоны Гаусса-Крюгера (если точка рассматривается в системе координат
	// соседней зоны, то номер зоны следует присвоить вручную)
	int zone = (int) (dLon/6.0 + 1);

	// Параметры эллипсоида Красовского
	double a = 6378245.0;		// Большая (экваториальная) полуось
	double b = 6356863.019;		// Малая (полярная) полуось

	double PI = 3.14159265358979;	// число Пи

	double e2 = ( pow(a, 2) - pow(b, 2) ) / pow(a, 2);	// Эксцентриситет
	double n = (a - b) / (a + b);						//	Приплюснотость

	// Параметры зоны Гаусса-Крюгера
	double F = 1.0;				// Масштабный коэффициент
	double Lat0 = 0.0;			// Начальная параллель в радианах

	double Lon0 = (zone*6-3)* PI / 180;		// Центральный меридиан (в радианах)

	double N0 = 0.0;					// Условное северное смещение для начальной параллели
	double E0 = zone * 1E6 + 500000.0;	// Условное восточное смещение для центрального меридиана
	
	// Перевод широты и долготы в радианы
	double Lat = dLat * PI / 180.0;
	double Lon = dLon * PI / 180.0;

	// Вычисление переменных для преобразования
	double sinLat = sin(Lat);
	double cosLat = cos(Lat);
	double tanLat = tan(Lat);

	double v = a * F * pow(1 - e2 * pow(sinLat, 2), -0.5);
	double p = a * F * (1-e2) * pow(1 - e2 * pow(sinLat, 2), -1.5);

	double n2 = v/p - 1;
	double M1 = (1 + n + 5.0/4.0 * pow(n,2) + 5.0/4.0 * pow(n, 3)) * (Lat-Lat0);
	double M2 = (3 * n + 3 * pow(n, 2) + 21.0/8.0 * pow(n, 3)) * sin(Lat - Lat0) * cos(Lat + Lat0);
	double M3 = (15.0/8.0 * pow(n, 2) + 15.0/8.0 * pow(n, 3)) * sin(2 * (Lat - Lat0) ) * cos(2 * (Lat + Lat0) );
	double M4 = 35.0/24.0 * pow(n, 3) * sin(3 * (Lat - Lat0)) * cos(3 * (Lat + Lat0));
	double M = b * F * (M1 - M2 + M3 - M4);

	double I = M + N0;
	double II = v/2 * sinLat * cosLat;
	double III = v/24 * sinLat * pow(cosLat, 3) * (5 - pow(tanLat, 2) + 9 * n2);
	double IIIA = v/720 * sinLat * pow(cosLat, 5) * (61 - 58 * pow(tanLat, 2) + pow(tanLat, 4));
	double IV = v * cosLat;
	double V = v/6 * pow(cosLat, 3) * (v/p - pow(tanLat, 2));
	double VI = v/120 * pow(cosLat, 5) * (5 - 18 * pow(tanLat, 2) + pow(tanLat, 4) + 14 * n2 - 58 * pow(tanLat, 2) * n2);

	// Вычисление северного и восточного смещения (в метрах)
	double N = I + II * pow(Lon - Lon0, 2) + III * pow(Lon - Lon0, 4) + IIIA * pow(Lon - Lon0, 6);
	double E = E0 + IV * (Lon - Lon0) + V * pow(Lon - Lon0, 3) + VI * pow(Lon - Lon0, 5);

	*pY = N;
	*pX = E;

	// Если Широта: 55.752;
	//		Долгота: 37.618;
	// то Северное смещение составляет:
	//		6181924.245933299
	// а Восточное смещение:
	// 7413223.481449484
}
//-------------------------------------------
// Преобразование геодезических координат из системы П3-90.02 в систему WGS-84 
// пересчет широты, долготы и высоты	(наиболее точный вариант преобразования [локальный вариант])
int P3_90_02_WGS_84(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_wgs_84, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_wgs_84, Y_wgs_84, Z_wgs_84, h_wgs;
	GeoCalc geo_calc;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX =	-0.035;
	const double dY =	-0.266;
	const double dZ =	-0.912;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx =	 -0.00000005;
	const double Wy =	 -0.00000005;
	const double Wz =	 -0.0000012;
	//-------------------------------
	const double m = 0.0000009;
	//-------------------------------
	const double Pi = 3.14159265358979;		// число Пи
	const double DEG_RAD = Pi/180.0;
	const double RAD_DEG = 180.0/Pi;
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_wgs_84 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_wgs_84 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_wgs_84 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_wgs_84) > 0.000001 && (abs(Y_wgs_84/X_wgs_84) < Pi/2 - 0.000001 || abs(Y_wgs_84/X_wgs_84) > Pi/2 + 0.000001) )
		L = atan(Y_wgs_84/X_wgs_84);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(geo_calc.Find_Root(lat1, lat2, cosLon, Z_wgs_84, e_2_wgs_84, a_wgs_84, X_wgs_84, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_wgs_84 = a_wgs_84 / sqrt(1 - e_2_wgs_84 * pow(sinLat, 2) ); 
		h_wgs = X_wgs_84/(cosLat * cosLon) - N_wgs_84;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//-------------------------------------------
// Преобразование геодезических координат из системы WGS-84 в систему П3-90.02
// пересчет широты, долготы и высоты
int WGS_84_P3_90_02(double Lat, double Lon, double H, double *ph_wgs, double *pLatOut, double *pLonOut)
{
	int res;
	double B, L, sinLat, cosLat, cosLon, sinLon, sin2Lat, N_p3_90_02, N_wgs_84, X_p3_90_02, Y_p3_90_02, Z_p3_90_02, X_wgs_84, Y_wgs_84, Z_wgs_84, h_wgs;
	GeoCalc geo_calc;
	const double a_wgs_84 = 6378137.0;							// размер большой полуоси в системе WGS-84 в метрах
	const double a_p3_90_02 = 6378136.0;						// размер большой полуоси в системе П3-90.02 в метрах
	//-----------------------------
	const double alfa_wgs_84 = 1/298.3;							// сжатие эллипсоида Красовского в системе WGS-84
	const double alfa_p3_90_02 = 1/298.25784;				// сжатие эллипсоида  в общеземной геодезической системе координат П3-90.02
	//-----------------------------
	const double e_2_wgs_84 = 2 * alfa_wgs_84 - pow(alfa_wgs_84, 2);					// квадрат эксцентриситета эллипсоида в системе WGS-84
	const double e_2_p3_90_02 = 2 * alfa_p3_90_02 - pow(alfa_p3_90_02, 2);		// квадрат эксцентриситета эллипсоида в системе П3-90.02
	//-------------------------------
	// линейные элементы трансформирования систем координат
	const double dX =	0.035;
	const double dY =	0.266;
	const double dZ =	0.912;
	//-------------------------------
	// угловые элементы трансформирования систем координат
	const double Wx =	 0.00000005;
	const double Wy =	 0.00000005;
	const double Wz =	 0.0000012;
	//-------------------------------
	const double m = -0.0000009;
	//-------------------------------
	const double Pi = 3.14159265358979;		// число Пи
	const double DEG_RAD = Pi/180.0;
	const double RAD_DEG = 180.0/Pi;
	const double ro = 206264.8062;								// число угловых секунд в радиане
	B = Lat * DEG_RAD;												// широта в радианах
	L = Lon * DEG_RAD;												// долгота в радианах
	sinLat = sin(B);
	cosLat = cos(B);
	cosLon = cos(L);
	sinLon = sin(L);

	sin2Lat = pow(sinLat, 2);
	//double M = a * (1 - e2 ) * pow( (1 - e2 * sin2Lat), -1.5 );		// радиус кривизны меридиана
	//double N = a * pow((1 - e2 * sin2Lat), -0.5);						// радиус кривизны первого вертикала
	N_p3_90_02 = a_p3_90_02 * pow((1 - e_2_p3_90_02 * sin2Lat), -0.5);

	res = 1;		// пока все Ок!

	//------------------------------------------------
	// реализация через переход к пространственным координатам X, Y, Z
	X_p3_90_02 = (N_p3_90_02 + H) * cosLat * cosLon;
	Y_p3_90_02 = (N_p3_90_02 + H) * cosLat * sinLon;
	Z_p3_90_02 = (N_p3_90_02 * (1 - e_2_p3_90_02) + H) * sinLat;

	X_wgs_84 = (1 + m) * (X_p3_90_02  + Wz * Y_p3_90_02 + Wy * Z_p3_90_02) + dX;
	Y_wgs_84 = (1 + m) * (-X_p3_90_02 * Wz + Y_p3_90_02 + Wx * Z_p3_90_02)  + dY;
	Z_wgs_84 = (1 + m) * (Wy * X_p3_90_02 - Wx * Y_p3_90_02 + Z_p3_90_02) + dZ;

	//-----------------------------------------------------
	double lat1 = 59.0;
	double lat2 = 61.0;
	
	L = 0;
	
	if(abs(X_wgs_84) > 0.000001 && (abs(Y_wgs_84/X_wgs_84) < Pi/2 - 0.000001 || abs(Y_wgs_84/X_wgs_84) > Pi/2 + 0.000001) )
		L = atan(Y_wgs_84/X_wgs_84);
	else
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	sinLon = sin(L);
	cosLon = cos(L);

	if(abs(sinLon) < 0.000001 || abs(cosLon) < 0.000001)
	{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
	}

	if(geo_calc.Find_Root(lat1, lat2, cosLon, Z_wgs_84, e_2_wgs_84, a_wgs_84, X_wgs_84, &B) )
	{
		*pLatOut = B;
		B = B * Pi / 180;
		sinLat = sin(B);
		cosLat = cos(B);

		if( abs(cosLat) < 0.000001)
		{
			res = 0;
			*pLonOut = -100000.0;
			*pLatOut = -100000.0;
			*ph_wgs = -100000.0;
			return res;
		}

		N_wgs_84 = a_wgs_84 / sqrt(1 - e_2_wgs_84 * pow(sinLat, 2) ); 
		h_wgs = X_wgs_84/(cosLat * cosLon) - N_wgs_84;
		
		*pLonOut = L * RAD_DEG;// 57.29578049;		// (перевод в градусах)
		*ph_wgs = h_wgs;
	}
	else
	{
		res = 0;
		*pLonOut = -100000.0;
		*pLatOut = -100000.0;
		*ph_wgs = -100000.0;
	}	

	return res;
}
//------------------------------------------
// Приведение географических координат (широты/долготы) к локальным координатам в системе КТА
// с использованием проекции Меркатора/WGS84
std::string LatLong2UTMMerc(double lon, double lat, double east_ref_point, double north_ref_point, double *pEasting, double *pNorthing, double *pX, double *pY)
{
	double X, Y, dX, dY;	// , Bx, By;
	std::string str_utm;
	GeoCalc geo_calc;
	str_utm = geo_calc.LatLong2UTMMerc(lon, lat, pEasting, pNorthing);
	
	X = *pEasting;
	Y = *pNorthing;
	//-------------------------------
	dX = X - east_ref_point;
	dY = Y - north_ref_point;

	/*Bx = 17.221975;
	By = -1.02839;*/

	// *pX = dX * 1.03;		// / 1.05356;	// так будет для Пулково (для разницы восточных смещений)
	// *pX = dX * 1.01941 + Bx;
	// *pX = dX * 0.968176 + 25; 
	*pX = dX; 

	//if(abs(dY) > 1080)
	//	*pY = dY / 1.06623;	// так будет для Пулково (для разницы северных смещений)
	//else
	//	*pY = dY / 1.29;
	
	//  *pY = dY / 1.09623;
	// *pY = dY / 1.07623;
	// *pY = dY * 1.03356 + By;
	// *pY = dY * 1.0557432;
	*pY = dY;

	return str_utm;
}
//-----------------------------------------
// переводит UTM в географические координаты WGS-84 (широту и долготу) 
void UTM2LatLongMerc(double kta_x, double kta_y, double east_ref_point, double north_ref_point, double *pLat, double *pLon)
{
	double Y, X, easting, northing;		//, Bx, By;
	GeoCalc geo_calc;
	std::string longZone = "36";
	std::string latZone = "V";

	/*Bx = 17.221975;
	By = -1.02839;*/

	// easting = kta_x * 1.05356 + east_ref_point;				// так будет для Пулково
	// easting = kta_x / 1.03 + east_ref_point;					// так будет для Пулково
	// easting = (kta_x - Bx) * 0.98096 + east_ref_point;		// так будет для Пулково
	// easting = kta_x * 1.03287 + east_ref_point;				// так будет для Пулково
	// easting = (kta_x - 25) * 1.03287 + east_ref_point;	// так будет для Пулково
	easting = kta_x + east_ref_point;

	// northing = kta_y * 1.09623 + north_ref_point;		// так будет для Пулково
	//if( abs(kta_y) > 1060 )
	//	northing = kta_y * 1.06623 + north_ref_point;		// так будет для Пулково
	//else
	//	northing = kta_y * 1.29 + north_ref_point;
	// northing = (kta_y - By)  * 0.96753 + north_ref_point;
	// northing = kta_y * 0.9472 + north_ref_point;
	northing = kta_y + north_ref_point;

	geo_calc.UTM2LatLongMerc(longZone, latZone, easting, northing, pLat, pLon);
}
//----------------------------------------
