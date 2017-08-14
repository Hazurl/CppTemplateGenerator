# templaeFileGenerator

File Generator, by Hazurl
Replace all selected keyword of a file by a specific value
```
-t=... : path to templated file (mandatory)
-p=... : path to output file (mandatory)
-b=... : start keyword delimiter (by default "{{")
-e=... : end keyword delimiter (by default "}}")
```
To specify keyword to be replaced `keyword=value`

The templated file need to write : `{{keyword}}`
There is two modifier available : `{{keyword:upper}}` and `{{keyword:lower}}`

## Example :

templated_file.txt :
```
	This is a {{file}}, that can be either {{black:upper}} or {{white:lower}}.
```
Command : 
```
	fgen -p=output.txt -t=templated_file.txt file=JokE black=FUnny white=nOT
```
ouput.txt : 
```
	This is a JokE, that can be either FUNNY or not.
```
