String pad64(String hex)
{
    while (hex.length() < 64)
        hex = "0" + hex;
    return hex;
}

String hexToAscii(String hex)
{
    String result = "";
    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        String byteStr = hex.substring(i, i + 2);
        char c = (char)strtol(byteStr.c_str(), nullptr, 16);
        if (c != 0)
            result += c;
    }
    return result;
}

String weiToEth(String hexWei)
{
    // Elimina el prefijo "0x"
    hexWei.replace("0x", "");
    // Convertir a decimal usando double (solo para mostrar)
    double wei = strtoull(hexWei.c_str(), nullptr, 16);
    double eth = wei / 1e18;
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.6f ETH", eth);
    return String(buffer);
}