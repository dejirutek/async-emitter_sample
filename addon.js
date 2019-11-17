'use strict'

const { EventEmitter } = require('events');
const { inherits } = require('util');

// アドオン初期化
const { AsyncEmitter } = require('bindings')('async_emitter');

// イベント発生メソッドを継承させる
inherits(AsyncEmitter, EventEmitter);

// 引き数は、イベント発生インターバル（秒）
const emitter = new AsyncEmitter(1);

// 'data' イベントリスナー
emitter.on('data', (data, len) => {
    console.log('event data =', data, ' len =', len);
});

let iLength = 8;
let iCount = 5;

// Workerパラメータ初期化
emitter.AsyncInit(iLength, iCount);

// キューに投入
emitter.AsyncQueue();
