
## **Cypher**

  

[**https://z1labs.ai/cypher/**](https://z1labs.ai/cypher/)

  

---

  

📃 [Read white paper](#) | 📒 [Documentation](#) | 🛸 [Community support](#)

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

**Cypher**, FHE-EVM Layer is the core of the DeAI ecosystem, designed to optimize blockchain for AI applications. It integrates Fully Homomorphic Encryption (FHE) with Ethereum Virtual Machine (EVM) compatibility, allowing encrypted AI computation without sacrificing performance. This combination ensures that data remains secure while being processed by AI, maintaining both confidentiality and efficiency.

The FHE-EVM integration supports encrypted data processing within smart contracts, enabling secure AI execution on-chain. It includes precompiled cryptographic contracts that embed FHE operations to optimize execution and reduce computational overhead. Additionally, FHE gas optimization adjusts gas calculations specifically for FHE operations, ensuring efficient resource use.


## Table of Contents

- [Overview](#overview)

- [Features](#features)

- [Architecture](#architecture)

- [Examples](#examples)

- [Documentation](#documentation)

## Overview
Homomorphic encryption is fundamentally designed to enable direct computation of data while it remains encrypted. This means the information can stay confidential throughout its processing, allowing valuable tasks to be conducted even when working with data stored in untrusted or distributed environments. In today’s world of distributed computing and heterogeneous networks, this capability is exceptionally important.

A homomorphic cryptosystem shares similarities with other public-key encryption methods: it uses a public key for encryption and restricts decryption to the private key holder. Its key distinction, however, lies in its algebraic framework, which makes it possible to carry out various types of computations on the encrypted data itself.



### Why Is This a Revolution? - Because It Enables Decentralized Blockchain Scaling with Encrypted Transactions!
Previously, encrypted transactions were difficult to process at scale because nodes couldn't perform computations on encrypted data. Homomorphic encryption solves this by allowing computations on encrypted transactions without needing to reveal the underlying data. This is revolutionary because the nodes no longer require the private key used to encrypt the information in order to process it. As a result, blockchain networks can scale in a fully decentralized manner while maintaining the confidentiality of the data.

In this system, separate gateway nodes act as decryptors. Upon request, they verify user access, decrypt the data, re-encrypt it with the user's public key for secure transmission, and deliver it back. The user can then decrypt the data using their private key, preserving confidentiality throughout the process.


## Features

  
-  **Solidity integration:** fhEVM contracts are simple solidity contracts that are built using traditional solidity toolchains.

-  **FHEVM Encryption:** Cypher integrates fully homomorphic encryption (FHE) into smart contracts, allowing computations to be performed on encrypted data without ever needing to decrypt it, ensuring maximum privacy and security.

-  **Optimized Rollup Technology:** With Rollup design, Cypher achieves faster execution and lower transaction costs, making it ideal for high-throughput decentralized applications.

-  **Decentralized Layer 3 Deployment:** Utilizing Rollup, Cypher enables decentralized deployment of Layer 3 solutions, giving developers more control and flexibility in building applications.

-  **Privacy-Preserving Computations**: By using FHE, Cypher ensures that all computations are private, allowing sensitive data to remain encrypted while still enabling meaningful operations, ideal for industries requiring high levels of data confidentiality.

-  **Low-Cost Transactions**: Cypher benefits from Rollup Layer 2 infrastructure, providing a low-cost environment for executing smart contracts without sacrificing security or decentralization.

-  **Scalability**: Cypher enhances the scalability of Ethereum, allowing more complex and resource-intensive decentralized applications (dApps) to run seamlessly on its platform.

  

## Architecture

  

### Layered Structure

  

-  **Layer 2 (L2) - Rollup:** Acts as the parent chain, utilizing optimized rollup technology for efficient execution. It batches and processes transactions off-chain, offering high throughput and low fees while maintaining Ethereum's security model.

-  **Layer 3 (L3) - Cypher with FHEVM Integration**: Built on Rollup, Cypher introduces FHE capabilities into smart contracts, enabling secure and private computations on encrypted data.

  

### FHEVM Integration

  

-  **fhevm-go Library**: Cypher integrates the fhevm-go library into the go-ethereum client, a component of Rollup. This integration allows smart contracts to perform computations on encrypted data seamlessly, leveraging fully homomorphic encryption capabilities.

-  **Containerized Execution**: The modified go-ethereum client runs within a Docker container under rollup. This setup ensures isolated and efficient execution of Layer 3 operations, enhancing scalability and security.

  

### Encryption and Decryption Services

  

-  **Key Management Service (KMS):** Validators do not possess the blockchain's private key; instead, a dedicated KMS securely manages it. This approach enhances security by preventing any single validator from accessing sensitive key material.

-  **Gateway Service:** Acts as an intermediary for decryption and re-encryption requests, handling two primary scenarios:

-  **Smart Contract Decryption:** When a smart contract requires plaintext data, the Gateway listens for decryption request events and returns the decrypted value via a callback function.

-  **dApp Re-encryption:** For decentralized applications needing plaintext, the Gateway provides an API to re-encrypt ciphertext with the dApp's public key, allowing the dApp to decrypt it locally.

  

  

### Precompiles

-  **Access Control List (ACL):** Defines which addresses can manipulate or access ciphertexts, ensuring unauthorized users can't access sensitive data.

-  **TFHE Executor:** Manages all encrypted data operations through the TFHEExecutor interface, enabling secure computations on encrypted data.

-  **KMS Verifier:** Handles the verification of signatures, ensuring the validity of transactions by managing verifiers.

-  **Gateway:** Acts as the intermediary for encryption and decryption requests, relaying them between the smart contract and the Key Management Service (KMS), which provides decryption results securely.

  

## Process

### Re-encryption

  

-  **Ciphertext Retrieval:** The smart contract implements a view function (e.g., balanceOf) to return the encrypted data (ciphertext) that needs to be re-encrypted. This ciphertext is initially encrypted with the FHE blockchain key.

-  **Client-Side Setup:** The dApp initiates a connection to the FHEVM network via fhevmjs and generates a  public and private keys to be used for the re-encryption process.

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

> 💡 **Attention**:: Once decrypted on smart contract, information becomes accessible to everyone on the blockchain. Therefore, strict access controls are essential.

  

#
  ![Async Decrypt](https://github.com/user-attachments/assets/0b3d11db-e95f-45e1-af78-6cce89f0216d?width=400&dpr=3&quality=100)


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

Check the documentation for a step-by-step guide on how to run your own

- [Build and deploy smart contracts]

- [Use Metamask Cypher snap to for an easy access to decryption service]

  
  

[Build and deploy smart contracts]: <https://github.com/z1labs/Cypher>

[Use Metamask Cypher snap to for an easy access to decryption service]: <https://github.com/z1labs/Cypher>
