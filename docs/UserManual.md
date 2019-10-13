# 用户手册

<!-- TOC -->

- [用户手册](#用户手册)
    - [1 文件格式说明](#1-文件格式说明)
    - [2 API说明](#2-api说明)
    - [2.1 添加向量至索引](#21-添加向量至索引)
        - [2.1.1 请求](#211-请求)
        - [2.1.2 返回](#212-返回)
        - [2.1.3 成功请求返回值示例](#213-成功请求返回值示例)
        - [2.1.4 失败请求返回值示例](#214-失败请求返回值示例)
        - [2.1.5 ERROR_MESSAGE](#215-error_message)
    - [2.2 查询向量](#22-查询向量)
        - [2.2.1 请求](#221-请求)
        - [2.2.2 返回](#222-返回)
        - [2.2.3 成功请求返回值示例](#223-成功请求返回值示例)
        - [2.2.4 失败请求返回值示例](#224-失败请求返回值示例)
        - [2.2.5 ERROR_MESSAGE](#225-error_message)
    - [2.3 重新配置和加载索引文件](#25-重新配置和加载索引文件)
        - [2.3.1 请求](#231-请求)
        - [2.3.2 返回](#232-返回)
        - [2.3.3 成功请求返回值示例](#233-成功请求返回值示例)
        - [2.3.4 失败请求返回值示例](#234-失败请求返回值示例)
        - [2.3.5 ERROR_MESSAGE](#235-error_message)

<!-- /TOC -->

## 1 文件格式说明

文件中前20个字节为文件头，前12个字节顺序存储向量维数、向量个数、文件格式版本号，存储类型为 `unsigned int`,文件头最后8个字节为空。

**之后是(向量个数)个`unsigned int`顺序表示向量的id，然后是(向量个数*向量维数)个float表示向量**

| 名称 | 类型 | 参数说明 |
| :----: | :--: | :------: |
| header[0] | `unsigned int` |  向量维数 |
| header[1] | `unsigned int` | 向量个数 |
| header[2] | `unsigned int` | 文件格式版本号 |
| id | `unsigned int` | 向量唯一标识 |
| feature | `float` | 特征向量 |

## 2 API说明

## 2.1 添加向量至索引

### 2.1.1 请求

调用地址：http://server/add
请求方式：POST
请求类型：application/json

| 是否必选 | 参数名 | 类型 | 参数说明 |
| :------: | :----: | :--: | :------: |
| 必选 | count | `Int` | 需要添加的向量个数 |
| 必选 | vectors | `Object` | 向量id及对应特征向量 |

**请求参数结构示例**

```json
{
	"count": 100,
  	"vectors":
        {
            "0": [0.1,...,0.5],
            ...,
            "99": [0.1,...,0.5]
        }
}
```

### 2.1.2 返回

返回类型：JSON

| 参数名 | 类型 | 参数说明 |
| :----: | :--: | :------: |
| code | `Int` | HTTP状态码 |
| time_used | `Int` | 整个请求所花费的时间，单位为毫秒 |
| error_message | `String` | 当请求失败时返回错误信息，请求成功时返回请求结果。 |

### 2.1.3 成功请求返回值示例

```json
{
    "code": 200,
	"time_used": 50,
  	"error_message": "ADD_SUCCESS"
}
```

### 2.1.4 失败请求返回值示例

```json
{
    "code": 400,
	"time_used": 0,
  	"error_message": "INVALID_COUNT"
}
```

### 2.1.5 ERROR_MESSAGE

| HTTP状态代码 | 错误信息 | 说明 |
| :----: | :--: | :------: |
| 400 | INVALID_COUNT | 参数count格式不正确 |
| 400 | INVALID_VECTORS | 参数vector格式不正确 |
| 400 | EMPTY_DATA | 参数vector中没有向量 |
| 400 | MISSING_ARGUMENTS | 缺少参数 |
| 400 | INVALID_DIMENSION | vector中向量维数不正确 |


## 2.2 查询向量

### 2.2.1 请求

调用地址：http://server/search
请求方式：POST
请求类型：application/json

| 是否必选 | 参数名 | 类型 | 参数说明 |
| :------: | :----: | :--: | :------: |
| 必选 | count | `Int` | 需要查询的向量个数 |
| 必选 | topk | `Int` | 查询最近邻的个数 |
| 必选 | vectors | `Object` | 需要查询的向量id及对应特征向量 |

**请求参数结构示例**

```json
{
	"count": 10,
    "topk": 2,
  	"vectors":
    {
        "q0": [0.1,...,0.5],
        ...,
        "q9": [0.1,...,0.5]
    }
}
```

### 2.2.2 返回

返回类型：JSON

| 参数名 | 类型 | 参数说明 |
| :----: | :--: | :------: |
| code | `Int` | HTTP状态码 |
| result | `Object` | 每个查询向量的k个最近邻距离及标识 |
| time_used | `Int` | 整个请求所花费的时间，单位为毫秒 |
| error_message | `String` | 当请求失败时返回错误信息，请求成功时返回请求结果。 |

### 2.2.3 成功请求返回值示例

```json
{
    "code":200,
	"result":
    {
        "q0": {"distance": [77.55, 78.39], "labels": [80, 1]},
        "q1": {"distance": [67.94, 71.17], "labels": [70, 49]},
		...,
        "q9": {"distance": [73.63, 73.78], "labels": [47, 28]}
    },
	"time_used": 50,
  	"error_message": "SEARCH_SUCCESS"
}
```

### 2.2.4 失败请求返回值示例

```json
{
    "code": 400,
	"time_used": 0,
  	"error_message": "EMPTY_QUERIES"
}
```

### 2.2.5 ERROR_MESSAGE

| HTTP状态代码 | 错误信息 | 说明 |
| :----: | :--: | :------: |
| 400 | INVALID_COUNT | 参数count格式不正确 |
| 400 | INVALID_TOPK | 参数topk格式不正确 |
| 400 | EMPTY_QUERIES | 参数vectors中没有向量 |
| 400 | INVALID_DIMENSION | vector中向量维数不正确 |
| 400 | INVALID_VECTORS_SIZE | vectors个数与count不符 |
| 400 | INVALID_VECTORS | vectors格式不正确 |

## 2.3 重新配置和加载索引文件

### 2.3.1 请求

调用地址：http://127.0.0.1/reconfig
请求方式：POST
请求类型：application/json
**不由业务端调用**

| 是否必选 | 参数名 | 类型 | 参数说明 |
| :------: | :----: | :--: | :------: |
| 必选 | reconfigFilePath | `String` | 需要重新加载的配置文件路径 |

**请求参数结构示例**

```json
{
    "reconfigFilePath": "config.json"
}
```

### 2.3.2 返回

返回类型：JSON

| 参数名 | 类型 | 参数说明 |
| :----: | :--: | :------: |
| time_used | `Int` | 整个请求所花费的时间，单位为毫秒 |
| error_message | `String` | 当请求失败时返回错误信息，请求成功时返回请求结果。 |

### 2.3.3 成功请求返回值示例

```json
{
	"time_used": 350,
  	"error_message": "RECONFIG_SUCCESS"
}
```

### 2.3.4 失败请求返回值示例

```json
{
	"time_used": 0,
  	"error_message": "INVALID_RECONFIG_FILEPATH"
}
```

### 2.3.5 ERROR_MESSAGE

| HTTP状态代码 | 错误信息 | 说明 |
| :----: | :--: | :------: |
| 400 | INVALID_RECONFIG_FILEPATH | 参数reconfigFilePath格式不正确 |
| 400 | RELOAD_FAIL | 重新加载数据失败 |
| 400 | READD_FAIL | 重新构建索引结构失败 |