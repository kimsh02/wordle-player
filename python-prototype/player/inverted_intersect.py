from game.wordle import Letter

class InvertedIntersect:

    inv_index = None
    valid_guesses = None
    guess = None
    response = None
    postings_bucket = None

    def __init__(self):
        pass

    def bucket(self):
        bucket = {}
        yellows = {}
        for color in Letter:
            bucket[color] = []
        grays = []
        for i in range(5):
            key = self.guess[i] + str(i)
            res = self.response[i]
            if res == Letter.GREEN:
                bucket[res].append((key, self.inv_index[key][0]))
            elif res == Letter.GRAY:
                if key[0] not in grays:
                    grays.append(key[0])
                    for i in range(5):
                        tmp = key[0]+str(i)
                        bucket[res].append((tmp, self.inv_index[tmp][0]))
            else:
                if key[0] not in yellows.keys():
                    yellows[key[0]] = []
                yellows[key[0]].append(key[1])
        for k,v in yellows.items():
            for pos in v:
                for i in range(5):
                    key = k + pos
                    tmp = k + str(i)
                    if tmp in self.inv_index.keys():
                        if tmp == key:
                            bucket[Letter.GRAY].append(
                                (tmp, self.inv_index[tmp][0]))
                        else:
                            bucket[Letter.YELLOW].append(
                                (tmp, self.inv_index[tmp][0]))
        bucket[Letter.GREEN].append(('vg', len(self.valid_guesses)))
        for v in bucket.values():
            self.sort_postings(v)
        self.postings_bucket = bucket

    def get_intersection(self, guess, response, inv_index, valid_guesses):
        '''
        Returns intersection based on guess (the color of letters)

        @Param guess: the five letter string guess
        @Param response: color encodings of letters
        @Param inv_index: inverted index
        @Param valid_guesses: list of valid guesses
        @Return: the intersection as a list
        '''
        self.inv_index = inv_index
        self.valid_guesses = valid_guesses
        self.guess = guess
        self.response = response
        self.bucket()
        # print(self.postings_bucket)
        greens = self.intersection(self.postings_bucket[Letter.GREEN])
        # print(greens)
        yellows = self.union(self.postings_bucket[Letter.YELLOW])
        # print(yellows)
        grays = self.union(self.postings_bucket[Letter.GRAY])
        # print(grays)

        minus = self.minus(greens, grays)
        if yellows:
            return self.intersection([minus, yellows])
        return minus

    def sort_postings(self, postings):
        postings.sort(key=lambda posting: posting[1])
                        
    def union(self, p):
        if not p:
            return []
        if len(p) == 1:
            return self.posting(p)
        p1 = self.posting(p)
        p2 = self.union(p[:-1])
        ans = []
        while p1 and p2:
            if p1[0] == p2[0]:
                ans.append(p1[0])
                p1 = p1[1:]
                p2 = p2[1:]
            elif p1[0] < p2[0]:
                ans.append(p1[0])
                p1 = p1[1:]
            else:
                ans.append(p2[0])
                p2 = p2[1:]
        while p1:
            ans.append(p1[0])
            p1 = p1[1:]
        while p2:
            ans.append(p2[0])
            p2 = p2[1:]
        return ans

    def minus(self, p1, p2):
        if not p2:
            return p1
        ans = []
        while p1 and p2:
            if p1[0] == p2[0]:
                p1 = p1[1:]
                p2 = p2[1:]
            elif p1[0] < p2[0]:
                ans.append(p1[0])
                p1 = p1[1:]
            else:
                p2 = p2[1:]
        while p1:
            ans.append(p1[0])
            p1 = p1[1:]
        return ans
        
    def intersection(self, p):
        if len(p) == 1:
            return self.posting(p)
        p1 = self.posting(p)
        p2 = self.intersection(p[:-1])
        ans = []
        while p1 and p2:
            if p1[0] == p2[0]:
                ans.append(p1[0])
                p1 = p1[1:]
                p2 = p2[1:]
            elif p1[0] < p2[0]:
                p1 = p1[1:]
            else:
                p2 = p2[1:]
        return ans

    def posting(self, p):
        if type(p[-1]) is tuple:
            posting = p[-1]
            if posting[0] == 'vg':
                return self.valid_guesses
            return self.inv_index[posting[0]][1:]
        return p[-1]