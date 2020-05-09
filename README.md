# IP Compress
## 1. What's this?
A command line tool that "compresses" a list of IPv4 addresses into a set of IPs and subnets that moderately well describe the original list. 

## 2. Why?
Efficient firewall rules mostly. If you have a list of IPs you need to white or black list and the list if especially long then the list can become difficult to manage.

## 3. Usage
The following would `ipcompress` a list of IPs (each on a new line) grouping by subnets where more than 20% of the subnet's IPs appear in the list.
```bash
./ipcompress iplist.txt 20
```
