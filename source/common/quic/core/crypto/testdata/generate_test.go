// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This program generates a number of testing certificates for QUIC unit tests.
//
// blaze build //gfe/quic/core/crypto/testdata:generate_test
// cd blaze build //gfe/quic/core/crypto/testdata:generate_test
// ../../../../../blaze-bin/gfe/quic/core/crypto/testdata/generate_test

package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/rsa"
	"crypto/sha256"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/pem"
	"math/big"
	"os"
	"os/exec"
	"time"
)

const nonceFilename = "nonce.txt"
const sslKeyEncryptorFilename = "../../../../../blaze-bin/gfe/gfe2/ssl_cert_storage/ssl_key_encryptor"

func main() {
	// This binary is a go_test, unavoidably.
	if os.Getenv("TEST_TMPDIR") != "" {
		return
	}
	h := sha256.New()
	h.Write([]byte("seed for deterministic randomness"))
	rand, err := NewAESRandom(h.Sum(nil)[:16])
	if err != nil {
		panic(err)
	}

	notBefore, err := time.Parse(time.UnixDate, "Mon Jan 1 10:00:00 UTC 2013")
	if err != nil {
		panic(err)
	}

	notAfter, err := time.Parse(time.UnixDate, "Mon Dec 31 10:00:00 UTC 2023")
	if err != nil {
		panic(err)
	}

	rootKey, err := rsa.GenerateKey(rand, 2048)
	if err != nil {
		panic(err)
	}
	keyToFile("root.key", rootKey)

	rootTemplate := x509.Certificate{
		SerialNumber: new(big.Int).SetInt64(1),
		Subject: pkix.Name{
			Organization: []string{"Acme Co"},
			CommonName:   "Root CA",
		},
		NotBefore:             notBefore,
		NotAfter:              notAfter,
		KeyUsage:              x509.KeyUsageCertSign,
		ExtKeyUsage:           []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		BasicConstraintsValid: true,
		IsCA: true,
	}

	derBytes, err := x509.CreateCertificate(rand, &rootTemplate, &rootTemplate, &rootKey.PublicKey, rootKey)
	if err != nil {
		panic(err)
	}
	certToFile("root.crt", derBytes)

	intermediateKey, err := rsa.GenerateKey(rand, 2048)
	if err != nil {
		panic(err)
	}
	keyToFile("intermediate.key", intermediateKey)

	intermediateTemplate := x509.Certificate{
		SerialNumber: new(big.Int).SetInt64(2),
		Subject: pkix.Name{
			Organization: []string{"Acme Co"},
			CommonName:   "Intermediate CA",
		},
		NotBefore:             notBefore,
		NotAfter:              notAfter,
		KeyUsage:              x509.KeyUsageCertSign,
		ExtKeyUsage:           []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		BasicConstraintsValid: true,
		IsCA: true,
	}

	derBytes, err = x509.CreateCertificate(rand, &intermediateTemplate, &rootTemplate, &intermediateKey.PublicKey, rootKey)
	if err != nil {
		panic(err)
	}
	certToFile("intermediate.crt", derBytes)

	leafKey, err := rsa.GenerateKey(rand, 2048)
	if err != nil {
		panic(err)
	}
	keyToFile("test.example.com.key", leafKey)
	encryptKeyFile("test.example.com.key", sslKeyEncryptorFilename, nonceFilename)

	leafTemplate := x509.Certificate{
		SerialNumber: new(big.Int).SetInt64(3),
		Subject: pkix.Name{
			Organization: []string{"Acme Co"},
			CommonName:   "test.example.com",
		},
		NotBefore:             notBefore,
		NotAfter:              notAfter,
		KeyUsage:              x509.KeyUsageDigitalSignature | x509.KeyUsageKeyEncipherment,
		ExtKeyUsage:           []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		BasicConstraintsValid: true,
		IsCA:     false,
		DNSNames: []string{"test.example.com"},
	}

	derBytes, err = x509.CreateCertificate(rand, &leafTemplate, &intermediateTemplate, &leafKey.PublicKey, intermediateKey)
	if err != nil {
		panic(err)
	}
	certToFile("test.example.com.crt", derBytes)

	ecdsaLeafKey, err := ecdsa.GenerateKey(elliptic.P256(), rand)
	if err != nil {
		panic(err)
	}
	ecdsaLeafFile, err := os.Create("test_ecc.example.com.key")
	if err != nil {
		panic(err)
	}
	ecdsaLeafKeyDER, err := x509.MarshalECPrivateKey(ecdsaLeafKey)
	if err != nil {
		panic(err)
	}
	pem.Encode(ecdsaLeafFile, &pem.Block{Type: "EC PRIVATE KEY", Bytes: ecdsaLeafKeyDER})
	ecdsaLeafFile.Close()
	encryptKeyFile("test_ecc.example.com.key", sslKeyEncryptorFilename, nonceFilename)

	ecdsaLeafTemplate := x509.Certificate{
		SerialNumber: new(big.Int).SetInt64(4),
		Subject: pkix.Name{
			Organization: []string{"Acme Co"},
			CommonName:   "test.example.com",
		},
		NotBefore:             notBefore,
		NotAfter:              notAfter,
		KeyUsage:              x509.KeyUsageDigitalSignature,
		ExtKeyUsage:           []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		BasicConstraintsValid: true,
		IsCA:     false,
		DNSNames: []string{"test.example.com"},
	}

	derBytes, err = x509.CreateCertificate(rand, &ecdsaLeafTemplate, &intermediateTemplate, &ecdsaLeafKey.PublicKey, intermediateKey)
	if err != nil {
		panic(err)
	}
	certToFile("test_ecc.example.com.crt", derBytes)
}

// keyToFile writes a PEM serialization of |key| to a new file called
// |filename|.
func keyToFile(filename string, key *rsa.PrivateKey) {
	file, err := os.Create(filename)
	if err != nil {
		panic(err)
	}
	defer file.Close()
	pem.Encode(file, &pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(key)})
}

func encryptKeyFile(keyFilename string, encryptorBinary string, nonceFilename string) {
	keystoreFilename := keyFilename + ".encrypted"
	args := []string{
		encryptorBinary,
		"--encrypt",
		"--input_file", keyFilename,
		"--nonce_file", nonceFilename,
		"--ssl_private_key_encryption_helper_uses_mock_keystore",
		"--output_file", keystoreFilename,
	}
	cmd := exec.Command(encryptorBinary, args...)
	if err := cmd.Run(); err != nil {
		panic(err)
	}
}

// certToFile writes a PEM serialization and OpenSSL debugging dump of
// |derBytes| to a new file called |filename|.
func certToFile(filename string, derBytes []byte) {
	cmd := exec.Command("openssl", "x509", "-text", "-inform", "DER")

	file, err := os.Create(filename)
	if err != nil {
		panic(err)
	}
	defer file.Close()
	cmd.Stdout = file
	cmd.Stderr = os.Stderr

	stdin, err := cmd.StdinPipe()
	if err != nil {
		panic(err)
	}

	if err := cmd.Start(); err != nil {
		panic(err)
	}
	if _, err := stdin.Write(derBytes); err != nil {
		panic(err)
	}
	stdin.Close()
	if err := cmd.Wait(); err != nil {
		panic(err)
	}
}

// AESRandom implements a deterministic random generator using AES in counter
// mode.
type AESRandom struct {
	cipher       cipher.Block
	buf, counter [aes.BlockSize]byte
	offset       int
}

func NewAESRandom(key []byte) (*AESRandom, error) {
	cipher, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}
	return &AESRandom{cipher: cipher}, nil
}

func (a *AESRandom) incCounter() {
	n := 1
	for i := 0; i < aes.BlockSize; i++ {
		n += int(a.counter[i])
		a.counter[i] = byte(n)
		n >>= 8
	}
}

func (a *AESRandom) Read(buf []byte) (int, error) {
	total := len(buf)

	if a.offset != 0 {
		n := copy(buf, a.buf[a.offset:])
		a.offset = (a.offset + n) % aes.BlockSize
		buf = buf[n:]
	}

	for len(buf) >= aes.BlockSize {
		a.cipher.Encrypt(buf, a.counter[:])
		a.incCounter()
		buf = buf[aes.BlockSize:]
	}

	if len(buf) > 0 {
		a.cipher.Encrypt(a.buf[:], a.counter[:])
		a.incCounter()
		a.offset = copy(buf, a.buf[:])
	}

	return total, nil
}
