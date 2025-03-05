class InvertedIndex:
    indices = None
    inv_index = None
    docs = None

    def __init__(self):
        pass

    def index(self, docs):
        '''
        Builds inverted index

        @Param docs: list of valid words to guess
        @Return: positional index
        '''
        self.docs = docs
        self.build_indices()
        self.inv_index = {}
        for index in self.indices:
            self.inv_index[index] = []
            pos = int(index[1])
            letter = index[0]
            for doc in self.docs:
                if letter == doc[pos]:
                    self.inv_index[index].append(doc)
            self.inv_index[index].insert(0, len(self.inv_index[index]))
        return self.inv_index

    def build_indices(self):
        s = set()
        for doc in self.docs:
            for i in range(5):
                s.add(doc[i] + str(i))
            if len(s) == 130:
                break
        self.indices = sorted(s)

    def print_indices(self):
        print(self.indices)

    def print_index(self):
        for k,v in self.inv_index.items():
            print(k)
            print(v)