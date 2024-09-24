
## **Cypher**

  

[**https://z1labs.ai/cypher/**](https://z1labs.ai/cypher/)

  

---

  

📃 [Read white paper](#) | 📒 [Documentation](#) | 🛸 [Community support](#)

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

  

**Cypher** is a Layer 3 (L3) solution built on Arbitrum, utilizing both Arbitrum Nitro and Arbitrum Orbit to enhance scalability and functionality. Arbitrum Nitro provides optimized rollup technology for efficient execution, while Arbitrum Orbit allows decentralized Layer 3 deployment. Together, they extend Ethereum’s capabilities with the security and low transaction costs of Arbitrum's Layer 2 (L2) infrastructure.

  

Cypher's standout feature is its use of FHEVM encryption, enabling fully homomorphic encryption (FHE) within smart contracts. This allows encrypted data to be processed without decryption, ensuring privacy and security throughout computation. By combining this privacy-focused approach with Arbitrum’s scalability, Cypher offers a unique solution for secure, scalable applications on Ethereum.

  

## Table of Contents

- [Overview](#overview)

- [Features](#features)

- [Architecture](#architecture)

- [Examples](#examples)

- [Documentation](#documentation)

## Overview
The purpose of homomorphic encryption is to allow computation on encrypted data. Thus data can remain confidential while it is processed, enabling useful tasks to be accomplished with data residing in untrusted environments. In a world of distributed computation and heterogeneous networking this is extremely valuable.

A homomorphic cryptosystem functions similarly to other types of public encryption in the sense that it utilizes a public key to encrypt data and only permits the person with the corresponding private key to access the unencrypted data. Its distinctiveness, however, lies in its use of an algebraic system that enables a range of computations (or operations) on the encrypted data.



### Why Is This a Revolution? - Because It Enables Decentralized Blockchain Scaling with Encrypted Transactions!
Previously, encrypted transactions were difficult to process at scale because nodes couldn't perform computations on encrypted data. Homomorphic encryption solves this by allowing computations on encrypted transactions without needing to reveal the underlying data. This is revolutionary because the nodes no longer require the private key used to encrypt the information in order to process it. As a result, blockchain networks can scale in a fully decentralized manner while maintaining the confidentiality of the data.

In this system, separate gateway nodes act as decryptors. Upon request, they verify user access, decrypt the data, re-encrypt it with the user's public key for secure transmission, and deliver it back. The user can then decrypt the data using their private key, preserving confidentiality throughout the process.


## Features

  

-  **Layer 3 (L3) Architecture:** Built on top of Arbitrum, Cypher leverages both Arbitrum Nitro and Arbitrum Orbit to enhance scalability and flexibility, while maintaining Ethereum’s security guarantees.

-  **Solidity integration:** fhEVM contracts are simple solidity contracts that are built using traditional solidity toolchains.

-  **FHEVM Encryption:** Cypher integrates fully homomorphic encryption (FHE) into smart contracts, allowing computations to be performed on encrypted data without ever needing to decrypt it, ensuring maximum privacy and security.

-  **Optimized Rollup Technology:** With Arbitrum Nitro’s efficient rollup design, Cypher achieves faster execution and lower transaction costs, making it ideal for high-throughput decentralized applications.

-  **Decentralized Layer 3 Deployment:** Utilizing Arbitrum Orbit, Cypher enables decentralized deployment of Layer 3 solutions, giving developers more control and flexibility in building applications.

-  **Privacy-Preserving Computations**: By using FHE, Cypher ensures that all computations are private, allowing sensitive data to remain encrypted while still enabling meaningful operations, ideal for industries requiring high levels of data confidentiality.

-  **Low-Cost Transactions**: Cypher benefits from Arbitrum’s Layer 2 infrastructure, providing a low-cost environment for executing smart contracts without sacrificing security or decentralization.

-  **Scalability**: Cypher enhances the scalability of Ethereum, allowing more complex and resource-intensive decentralized applications (dApps) to run seamlessly on its platform.

  

## Architecture

  

### Layered Structure

  

-  **Layer 2 (L2) - Arbitrum:** Acts as the parent chain, utilizing optimized rollup technology for efficient execution. It batches and processes transactions off-chain, offering high throughput and low fees while maintaining Ethereum's security model.

-  **Layer 3 (L3) - Cypher with FHEVM Integration**: Built on Arbitrum Orbit rollup, Cypher introduces FHE capabilities into smart contracts, enabling secure and private computations on encrypted data.

  

### FHEVM Integration

  

-  **fhevm-go Library**: Cypher integrates the fhevm-go library into the go-ethereum client, a component of Arbitrum Nitro. This integration allows smart contracts to perform computations on encrypted data seamlessly, leveraging fully homomorphic encryption capabilities.

-  **Containerized Execution**: The modified go-ethereum client runs within a Docker container under Arbitrum Orbit's rollup. This setup ensures isolated and efficient execution of Layer 3 operations, enhancing scalability and security.

  

### Encryption and Decryption Services

  

-  **Key Management Service (KMS):** Validators do not possess the blockchain's private key; instead, a dedicated KMS securely manages it. This approach enhances security by preventing any single validator from accessing sensitive key material.

-  **Gateway Service:** Acts as an intermediary for decryption and re-encryption requests, handling two primary scenarios:

-  **Smart Contract Decryption:** When a smart contract requires plaintext data, the Gateway listens for decryption request events and returns the decrypted value via a callback function.

-  **dApp Re-encryption:** For decentralized applications needing plaintext, the Gateway provides an API to re-encrypt ciphertext with the dApp's public key, allowing the dApp to decrypt it locally.

  
  

> 💡 **Attention**:: Once decrypted, information becomes accessible to everyone on the blockchain. Therefore, strict access controls are essential.

  

### Precompiles

-  **Access Control List (ACL):** Defines which addresses can manipulate or access ciphertexts, ensuring unauthorized users can't access sensitive data.

-  **TFHE Executor:** Manages all encrypted data operations through the TFHEExecutor interface, enabling secure computations on encrypted data.

-  **KMS Verifier:** Handles the verification of signatures, ensuring the validity of transactions by managing verifiers.

-  **Gateway:** Acts as the intermediary for encryption and decryption requests, relaying them between the smart contract and the Key Management Service (KMS), which provides decryption results securely.

  

## Process

### Re-encryption

  

-  **Ciphertext Retrieval:** The smart contract implements a view function (e.g., balanceOf) to return the encrypted data (ciphertext) that needs to be re-encrypted. This ciphertext is initially encrypted with the FHE blockchain key.

-  **Client-Side Setup:** The dApp initiates a connection to the FHEVM network via fhevmjs and generates a NaCl keypair (public and private keys) to be used for the re-encryption process.

-  **EIP712 Signature:** The dApp creates an EIP712 object containing the public key and contract details, which the user must sign. This ensures the re-encryption process is authorized by the user.

-  **Ciphertext Re-encryption:** The dApp retrieves the ciphertext from the contract and sends it, along with the user’s signature and public key, to the FHEVM Gateway service. The Gateway re-encrypts the ciphertext using the provided public key.

-  **Decryption:** The re-encrypted ciphertext is returned to the dApp, which can now decrypt it using the previously generated private key. This decrypted data can be securely used by the dApp while maintaining privacy throughout the process.

  

> 💡 **Attention**:: The user must have appropriate permissions set in the Access Control List (ACL) to allow re-encryption, ensuring that only authorized users can perform this operation.

  

### Decryption

-  **Request Decryption:** The dApp or smart contract calls Gateway.requestDecryption, passing the encrypted data (ciphertext), a callback function selector, and other parameters like maximum timestamp and signature verification requirements.

-  **Gateway Processing:** The Gateway contract emits an EventDecryption, which is picked up by a relayer. The relayer sends the ciphertext to the Key Management Service (KMS) for decryption. The KMS signature ensures authenticity.

-  **Callback Function:** Once decryption is complete, the Gateway contract calls the specified callback function in the requesting contract, passing the decrypted values. The contract must protect this callback using the onlyGateway modifier to ensure only the Gateway can trigger it.

-  **Asynchronous Fulfillment:** The decryption process can take time and is fulfilled asynchronously, usually within 2 blocks on the fhEVM testnet. The relayer ensures timely delivery of the decryption results, but the contract can specify a maximum timestamp for valid decryption results.

-  **Relayer and KMS:** The relayer simply forwards decryption requests, while the KMS signature verification ensures that the relayer cannot tamper with the decryption result.

  

#

![Async Decrypt](https://docs.zama.ai/~gitbook/image?url=https%3A%2F%2F4221220623-files.gitbook.io%2F%7E%2Ffiles%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FdjZLXIKWOxfECtlEOsO1%252Fuploads%252Fgit-blob-6658c0482a3f2f8329eb01b1938491ed402d35c5%252FasyncDecrypt.png%3Falt%3Dmedia&width=400&dpr=3&quality=100&sign=88438dae&sv=1)

  
  

> 💡 **Attention**:: The decryption operation is asynchronous. To use it, your contract must extend the GatewayCaller contract. This will import automatically the Gateway solidity library as well.

  

## Examples

  

### Counter

```

import "fhevm/lib/TFHE.sol";

  

contract Counter {

euint32 counter;

  

function add(einput valueInput, bytes calldata inputProof) public {

euint32 value = TFHE.asEuint32(valueInput, inputProof);

counter = TFHE.add(counter, value);

TFHE.allow(counter, address(this));

}

}

```

### Random

```

import "fhevm/lib/TFHE.sol";

  

contract Rand {

euint8 public value8;

  

constructor() payable {

TFHE.setFHEVM(FHEVMConfig.defaultConfig());

}

  

function generate8() public {

value8 = TFHE.randEuint8();

TFHE.allowThis(value8);

}

}

```

  

### Documentation

Check the documetation for a step by step guide how to run own

- [Build and deploy smart contracts]

- [Use Metamask Cypher snap to for an easy access to decryption service]

  
  

[Build and deploy smart contracts]: <https://github.com/z1labs/Cypher>

[Use Metamask Cypher snap to for an easy access to decryption service]: <https://github.com/z1labs/Cypher>