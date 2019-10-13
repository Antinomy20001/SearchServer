# 配置说明

|选项|参数类型|说明|
|:--|:--|:--|
|listenip|ipv4||
|port|int||
|httpThreads|int|处理http请求|
|logConfigParams|object|日志级别开关配置|
|dataFilePath|string|需要加载的数据文件路径|
|dimension|int|特征维数|
|searchFactory|string|faiss的索引结构类型|

```json
{
    "httpParams": {
        "httpThreads": 5,
        "listenIP": "localhost",
        "port": 23333
    },
    "logConfigParams": {
        "debugEnabled": false,
        "infoEnabled": false,
        "warningEnabled": false
    },
    "searchToolParams": {
        "dataFilePath": "data.bin",
        "dimension": 512,
        "searchFactory": "IDMap,Flat"
    }
}
```
