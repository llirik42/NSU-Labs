typedef int Type;

void sortArray(Type* array, size_t length){
    for (size_t i = 1; i < length; i++){
        Type key = array[i];

        size_t j = i - 1;
        for (; j >= 0 && key < array[j]; j--){
            array[j + 1] = array[j];
        }

        array[j + 1] = key;
    }
}
