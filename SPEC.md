# SPEC

## request structure

- 1 byte for type of request

### 1 - register

not signed:

- 4 bytes for username len
- len bytes of username
- 4 bytes for rsa_public_key len  
- len bytes of rsa_public_key  
- \n

### 2 - update

not signed:

- 4 bytes for username len
- len bytes of username

signed with rsa_private_key:

- 4 bytes of ip
- 4 bytes of port
- \n

### 3 - query

not signed:

- 4 bytes for username len
- len bytes of username
- \n

## response structure

### register

- 1 byte for status  
-1 - internal server error
0 - success  
1 - username already registered  
2 - rsa_public_key already registered


- \n

### update

- 1 byte for status  
-1 - internal server error
0 - success  
1 - failure
- \n

### query

- 1 byte for status  
-1 - internal server error
0 - success
1 - username not found
- 4 bytes of ip
- 2 bytes of port
- 4 bytes for rsa_public_key len
- len bytes of rsa_public_key
- 4 bytes for timestamp len
- len bytes of timestamp
- \n
