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
		char str[80];
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
	else if( Lat > 59.809198 && Lat < 59.8109165 && Lon > 30.2440705 && Lon < 30.264759)
		WgsToLocPlkv_zone_2_square_76(Lat, Lon, pX, pY);
	else
	{
		char str[80];
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
int WgsToLocPlkv_zone_4(double Lat, double Lon, double *pX, double *pY)
{
	int calc_completed = true;
	
	if(  Lat > 59.790809 &&  Lat < 59.7927555 &&  Lon > 30.278881 && Lon < 30.2827865)
		WgsToLocPlkv_zone_4_square_29(Lat, Lon, pX, pY);
	else if( Lat > 59.790798 && Lat < 59.7928095 && Lon > 30.2827865 && Lon < 30.2860265)
		WgsToLocPlkv_zone_4_square_30(Lat, Lon, pX, pY);
	else if( Lat > 59.7889375 && Lat < 59.790809 && Lon > 30.2788595 && Lon < 30.283001 )
		WgsToLocPlkv_zone_4_square_35(Lat, Lon, pX, pY);
	/*else if( Lat > 59.7945615 && Lat < 59.7965075 && Lon > 30.260945 && Lon < 30.2647245 )
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
		WgsToLocPlkv_zone_3_square_46(Lat, Lon, pX, pY);*/
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
	
	if(X > 9.999 && X < 11.0001 && Y > -0.0001 && Y < 1.0001  )
		PlkvLocToWgs_zone_1_square_11(X, Y, pLat, pLon);
	else if(X > 1.999 && X < 3.0001 && Y > 1.999 && Y < 3.0001)
		PlkvLocToWgs_zone_1_square_25(X, Y, pLat, pLon);
	else if(X > 2.999 && X < 4.0001 && Y > 1.999 && Y < 3.0001)
		PlkvLocToWgs_zone_1_square_26(X, Y, pLat, pLon);
	else if(X > 3.999 && X < 5.0001 && Y > 1.999 && Y < 3.0001)
		PlkvLocToWgs_zone_1_square_27(X, Y, pLat, pLon);
	else if(X > -0.0001 && X < 1.0001 && Y > 2.999 && Y < 4.0001)
		PlkvLocToWgs_zone_1_square_34(X, Y, pLat, pLon);
	else if(X > 0.999 && X < 2.0001 && Y > 2.999 && Y < 4.0001)
		PlkvLocToWgs_zone_1_square_35(X, Y, pLat, pLon);
	else if(X > 1.999 && X < 3.0001 && Y > 2.999 && Y < 4.0001)
		PlkvLocToWgs_zone_1_square_36(X, Y, pLat, pLon);
	else
	{
		char str[80];
		sprintf(str, "In Zone 1, the coordinates could not be determined, X = %8.6f, Y = %8.6f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_2(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = true;
	
	if(X > -1.0001 && X < 0.0001 && Y > -0.0001 && Y < 1.0001  )
		PlkvLocToWgs_zone_2_square_1(X, Y, pLat, pLon);
	else if(X > -6.001 && X < -4.999 && Y > 4.999 && Y < 6.0001)
		PlkvLocToWgs_zone_2_square_76(X, Y, pLat, pLon);
	else
	{
		char str[80];
		sprintf(str, "In Zone 2, the coordinates could not be determined, X = %8.6f, Y = %8.6f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_3(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = 1;
	
	if(X > -1.0001 && X < 0.0001 && Y > -1.0001 && Y < 0.0001  )
		PlkvLocToWgs_zone_3_square_1(X, Y, pLat, pLon);
	else if(X > -14.001 && X < -12.999 && Y > -1.0001 && Y < 0.0001)
		PlkvLocToWgs_zone_3_square_14(X, Y, pLat, pLon);
	else if(X > -1.0001 && X < 0.0001 && Y > -2.0001 && Y < -0.999)
		PlkvLocToWgs_zone_3_square_15(X, Y, pLat, pLon);
	else if(X > -1.0001 && X < 0.0001 && Y > -3.0001 && Y < -1.999)
		PlkvLocToWgs_zone_3_square_29(X, Y, pLat, pLon);
	else if(X > -3.0001 && X < -1.999 && Y > -3.0001 && Y < -1.999)
		PlkvLocToWgs_zone_3_square_31(X, Y, pLat, pLon);
	else if(X > -4.0001 && X < -2.999 && Y > -3.0001 && Y < -1.999)
		PlkvLocToWgs_zone_3_square_32(X, Y, pLat, pLon);	
	else if(X > -5.0001 && X < -3.999 && Y > -3.0001 && Y < -1.999)
		PlkvLocToWgs_zone_3_square_33(X, Y, pLat, pLon);	
	else if(X > -3.0001 && X < -1.999 && Y > -4.0001 && Y < -2.999)
		PlkvLocToWgs_zone_3_square_45(X, Y, pLat, pLon);
	else if(X > -4.0001 && X < -2.999 && Y > -4.0001 && Y < -2.999)
		PlkvLocToWgs_zone_3_square_46(X, Y, pLat, pLon);
	else
	{
		char str[80];
		sprintf(str, "In Zone 3, the coordinates could not be determined, X = %8.6f, Y = %8.6f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = false;
	}

	return calc_completed;
}
//-------------------------------------------
int PlkvLocToWgs_zone_4(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = 1;
	
	if(X > 3.999 && X < 5.0001 && Y > -5.0001 && Y < -3.999  )
		PlkvLocToWgs_zone_4_square_29(X, Y, pLat, pLon);
	else if(X > 4.999 && X < 6.0001 && Y > -5.0001 && Y < -3.999 )
		PlkvLocToWgs_zone_4_square_30(X, Y, pLat, pLon);
	else if(X > 3.999 && X < 5.0001 && Y > -6.0001 && Y < -4.999 )
		PlkvLocToWgs_zone_4_square_35(X, Y, pLat, pLon);
	else
	{
		char str[80];
		sprintf(str, "In Zone 4, the coordinates could not be determined, X = %8.6f, Y = %8.6f", X, Y); 
		ern::Log::getInstance().Write(str);
		calc_completed = 0;
	}

	return calc_completed;
}
//-------------------------------------------
int WgsToLocPlkv(double Lat, double Lon, double *pX, double *pY)
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
int PlkvLocToWgs(double X, double Y, double *pLat, double *pLon)
{
	int calc_completed = 1;
	if(X > -0.0001 && Y > -0.0001  )
		calc_completed = PlkvLocToWgs_zone_1(X, Y, pLat, pLon);
	else if(X < 0.0001 && Y > -0.0001)
		calc_completed = PlkvLocToWgs_zone_2(X, Y, pLat, pLon);
	else if(X < 0.0001 && Y < 0.0001)
		calc_completed = PlkvLocToWgs_zone_3(X, Y, pLat, pLon);
	else if( X > -0.0001 && Y < 0.0001 )
		calc_completed = PlkvLocToWgs_zone_4(X, Y, pLat, pLon);

	return calc_completed;
}
//-------------------------------------------