package precompiles

import (
	"bytes"
	"errors"
	"math/big"

	"github.com/ethereum/go-ethereum/common"
)

// A minimal FheLib precompile method representation for testing purposes
type FheLibMethod struct {
	// name of the fhelib function
	name string
	// A minimal run function to simulate execution
	runFunction func(input []byte) ([]byte, error)
}

// Struct to represent the minimal FheLib precompile contract
type FheLibPrecompile struct {
	Address common.Address
}

// Helper function to simulate packing arguments
func PackArgs(args ...interface{}) []byte {
	buf := new(bytes.Buffer)
	for _, arg := range args {
		switch v := arg.(type) {
		case *big.Int:
			b := v.Bytes()
			padded := make([]byte, 32-len(b))
			buf.Write(padded)
			buf.Write(b)
		case byte:
			buf.WriteByte(v)
		}
	}
	return buf.Bytes()
}

// Minimal test method for FheAdd (adds two big integers)
func (p *FheLibPrecompile) FheAdd(lhs, rhs *big.Int, flag byte) (*big.Int, error) {
	input := PackArgs(lhs, rhs, flag)
	return p.runFheMethod("fheAdd", input)
}

// Minimal test method for FheSub (subtracts two big integers)
func (p *FheLibPrecompile) FheSub(lhs, rhs *big.Int, flag byte) (*big.Int, error) {
	input := PackArgs(lhs, rhs, flag)
	return p.runFheMethod("fheSub", input)
}

// Simulates running an FheLib method and returning a result
func (p *FheLibPrecompile) runFheMethod(method string, input []byte) (*big.Int, error) {
	for _, m := range FhelibMethods {
		if m.name == method {
			result, err := m.runFunction(input)
			if err != nil {
				return nil, err
			}
			return new(big.Int).SetBytes(result), nil
		}
	}
	return nil, errors.New("method not found")
}

// List of minimal FheLib methods for testing
var FhelibMethods = []*FheLibMethod{
	{
		name: "fheAdd",
		runFunction: func(input []byte) ([]byte, error) {
			// Minimal logic: just return the sum of two uint256 values
			lhs := new(big.Int).SetBytes(input[:32])
			rhs := new(big.Int).SetBytes(input[32:64])
			sum := new(big.Int).Add(lhs, rhs)
			return sum.Bytes(), nil
		},
	},
	{
		name: "fheSub",
		runFunction: func(input []byte) ([]byte, error) {
			// Minimal logic: subtract the two uint256 values
			lhs := new(big.Int).SetBytes(input[:32])
			rhs := new(big.Int).SetBytes(input[32:64])
			diff := new(big.Int).Sub(lhs, rhs)
			return diff.Bytes(), nil
		},
	},
}
