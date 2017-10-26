/*
 * File: lookup_Table_Tem_NEW20170204_data.c
 *
 * Code generated for Simulink model 'lookup_Table_Tem_NEW20170204'.
 *
 * Model version                  : 1.71
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Sat Feb 04 09:40:35 2017
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->HC(S)12
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "lookup_Table_Tem_NEW20170204.h"
#include "lookup_Table_Tem_NEW20170204_private.h"

/* Block parameters (auto storage) */
P_lookup_Table_Tem_NEW2017020_T lookup_Table_Tem_NEW20170204_P = {
  /*  Variable: lookup_table_tem_NEW20170204
   * Referenced by: '<Root>/1-D Lookup Table'
   */
  { -198.0F, -197.0F, -196.0F, -195.0F, -194.0F, -193.0F, -192.0F, -191.0F,
    -190.0F, -189.0F, -188.0F, -187.0F, -186.0F, -185.0F, -184.0F, -183.0F,
    -182.0F, -181.0F, -180.0F, -179.0F, -178.0F, -177.0F, -176.0F, -175.0F,
    -174.0F, -173.0F, -172.0F, -171.0F, -170.0F, -169.0F, -168.0F, -167.0F,
    -166.0F, -165.0F, -164.0F, -163.0F, -162.0F, -161.0F, -160.0F, -159.0F,
    -158.0F, -157.0F, -156.0F, -155.0F, -154.0F, -153.0F, -152.0F, -151.0F,
    -150.0F, -149.0F, -148.0F, -147.0F, -146.0F, -145.0F, -144.0F, -143.0F,
    -142.0F, -141.0F, -140.0F, -139.0F, -138.0F, -137.0F, -136.0F, -135.0F,
    -134.0F, -133.0F, -132.0F, -131.0F, -130.0F, -129.0F, -128.0F, -127.0F,
    -126.0F, -125.0F, -124.0F, -123.0F, -122.0F, -121.0F, -120.0F, -119.0F,
    -118.0F, -117.0F, -116.0F, -115.0F, -114.0F, -113.0F, -112.0F, -111.0F,
    -110.0F, -109.0F, -108.0F, -107.0F, -106.0F, -105.0F, -104.0F, -103.0F,
    -102.0F, -101.0F, -100.0F, -99.0F, -98.0F, -97.0F, -96.0F, -95.0F, -94.0F,
    -93.0F, -92.0F, -91.0F, -90.0F, -89.0F, -88.0F, -87.0F, -86.0F, -85.0F,
    -84.0F, -83.0F, -82.0F, -81.0F, -80.0F, -79.0F, -78.0F, -77.0F, -76.0F,
    -75.0F, -74.0F, -73.0F, -72.0F, -71.0F, -70.0F, -69.0F, -68.0F, -67.0F,
    -66.0F, -65.0F, -64.0F, -63.0F, -62.0F, -61.0F, -60.0F, -59.0F, -58.0F,
    -57.0F, -56.0F, -55.0F, -54.0F, -53.0F, -52.0F, -51.0F, -50.0F, -49.0F,
    -48.0F, -47.0F, -46.0F, -45.0F, -44.0F, -43.0F, -42.0F, -41.0F, -40.0F,
    -39.0F, -38.0F, -37.0F, -36.0F, -35.0F, -34.0F, -33.0F, -32.0F, -31.0F,
    -30.0F, -29.0F, -28.0F, -27.0F, -26.0F, -25.0F, -24.0F, -23.0F, -22.0F,
    -21.0F, -20.0F, -19.0F, -18.0F, -17.0F, -16.0F, -15.0F, -14.0F, -13.0F,
    -12.0F, -11.0F, -10.0F, -9.0F, -8.0F, -7.0F, -6.0F, -5.0F, -4.0F, -3.0F,
    -2.0F, -1.0F, 0.0F, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F,
    10.0F, 11.0F, 12.0F, 13.0F, 14.0F, 15.0F, 16.0F, 17.0F, 18.0F, 19.0F, 20.0F,
    21.0F, 22.0F, 23.0F, 24.0F, 25.0F, 26.0F, 27.0F, 28.0F, 29.0F, 30.0F, 31.0F,
    32.0F, 33.0F, 34.0F, 35.0F, 36.0F, 37.0F, 38.0F, 39.0F, 40.0F },

  /*  Variable: lookup_table_voltage_NEW20170204
   * Referenced by: '<Root>/1-D Lookup Table'
   */
  { 0.034263581F, 0.0352497287F, 0.0357426517F, 0.0362354815F, 0.0372208431F,
    0.0377133787F, 0.0386981554F, 0.0391903967F, 0.0401745848F, 0.0406665355F,
    0.0416501388F, 0.0426333547F, 0.0431248136F, 0.0441074446F, 0.0450896844F,
    0.0460715368F, 0.0470529981F, 0.0475435816F, 0.0485244617F, 0.0495049506F,
    0.0504850522F, 0.051954478F, 0.052933611F, 0.0539123565F, 0.0548907146F,
    0.0558686852F, 0.0573349148F, 0.0583119206F, 0.0597767F, 0.060752742F,
    0.062216077F, 0.0636785477F, 0.0646530464F, 0.066114068F, 0.0675742328F,
    0.0690335333F, 0.0704919621F, 0.0719495341F, 0.0734062493F, 0.0753471851F,
    0.0768018886F, 0.0782557353F, 0.0801928565F, 0.0821284577F, 0.0835791528F,
    0.0855120867F, 0.0874435082F, 0.0893734F, 0.0913017839F, 0.0937101394F,
    0.0956351161F, 0.0975585803F, 0.0999608F, 0.102360658F, 0.104758173F,
    0.107153341F, 0.10954617F, 0.111936651F, 0.114802152F, 0.1171875F,
    0.120046847F, 0.122902848F, 0.125755504F, 0.129079401F, 0.131924838F,
    0.135240316F, 0.138551295F, 0.141857758F, 0.145159721F, 0.148927912F,
    0.152220279F, 0.155977517F, 0.160197467F, 0.163942352F, 0.168148428F,
    0.172347203F, 0.176538691F, 0.181187361F, 0.185827076F, 0.190457866F,
    0.195541456F, 0.200153589F, 0.205676481F, 0.210727975F, 0.216226563F,
    0.222169131F, 0.227641493F, 0.2340101F, 0.239908606F, 0.246244535F,
    0.253014326F, 0.25976488F, 0.266496271F, 0.274102092F, 0.281238198F,
    0.288796753F, 0.296773583F, 0.305164307F, 0.314842582F, 0.32185629F,
    0.331029981F, 0.340167761F, 0.349702388F, 0.359198064F, 0.369512886F,
    0.379781932F, 0.390855461F, 0.401876032F, 0.413264841F, 0.425016016F,
    0.437123567F, 0.449581355F, 0.46279493F, 0.479204327F, 0.48980695F,
    0.504001439F, 0.51891017F, 0.534119308F, 0.549621701F, 0.565803468F,
    0.582258344F, 0.599366307F, 0.617110789F, 0.635474861F, 0.654063463F,
    0.673617721F, 0.693367779F, 0.714040816F, 0.735243917F, 0.756958604F,
    0.77952224F, 0.802552044F, 0.826725662F, 0.850966752F, 0.876288652F,
    0.902311087F, 0.92933321F, 0.956655324F, 0.985225618F, 1.01434839F,
    1.04399085F, 1.07473695F, 1.10652542F, 1.13899612F, 1.17210233F, 1.20637333F,
    1.24116671F, 1.27726901F, 1.31404352F, 1.3514303F, 1.39015234F, 1.42959154F,
    1.46992373F, 1.51106F, 1.55315042F, 1.59609234F, 1.64001071F, 1.68457F,
    1.73031652F, 1.77668905F, 1.82378352F, 1.87187183F, 1.92061341F, 1.97024786F,
    2.02043986F, 2.07139921F, 2.12313F, 2.17530084F, 2.22822762F, 2.28157449F,
    2.33546495F, 2.38998795F, 2.44480777F, 2.5F, 2.55549026F, 2.61119866F,
    2.66725755F, 2.72344398F, 2.77965283F, 2.83596635F, 2.89225197F, 2.94854951F,
    3.00462937F, 3.06051207F, 3.11612225F, 3.17144537F, 3.2263844F, 3.28090072F,
    3.33677077F, 3.3883965F, 3.4412508F, 3.49343133F, 3.54490423F, 3.59562397F,
    3.64553714F, 3.69461918F, 3.74277091F, 3.7899909F, 3.83612657F, 3.88150692F,
    3.92574763F, 3.96890211F, 4.01097822F, 4.05197096F, 4.09183359F, 4.13055563F,
    4.16815F, 4.20459414F, 4.23989058F, 4.24096346F, 4.30701828F, 4.33886051F,
    4.36957026F, 4.39916134F, 4.42763615F, 4.45501709F, 4.48132229F, 4.50657225F,
    4.53078508F, 4.55398941F, 4.57621F, 4.59746885F, 4.61780119F, 4.63723421F,
    4.65579414F, 4.67351866F, 4.69043875F, 4.70658493F, 4.72199202F, 4.73669529F,
    4.75072765F, 4.76412439F, 4.77692032F, 4.78914976F, 4.80084848F, 4.81205034F,
    4.82279062F, 4.83310223F, 4.84301901F },
  -1.0F                                /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<Root>/Gain1'
                                        */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */