#pragma once

#if UI_BACKEND == UI_BACKEND_GYVER
#define FACTORY_NAME "Завод ИРБИС"
#define PREVIEW_ACP_TEXT "АЦП"
#define PREVIEW_COLUMN_TEMP "t"
#define PREVIEW_TEMP "Темп:"
#define PREVIEW_FIRST_RESERVE "Рез1:"
#define PREVIEW_SECOND_RESERVE "Рез2:"
#define PREVIEW_STREET "Улица:"
#else
#define FACTORY_NAME "Factory IRBIS"
#define PREVIEW_ACP_TEXT "ACP t"
#define PREVIEW_TEMP "Temp:"
#define PREVIEW_FIRST_RESERVE "Res1:"
#define PREVIEW_SECOND_RESERVE "Res2:"
#define PREVIEW_STREET "Street:"
#endif