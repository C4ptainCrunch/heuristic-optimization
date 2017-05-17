import multiprocessing
import json
from datetime import datetime

from exp1 import star_run, instances50

if __name__ == '__main__':
    jobs = [(instance, "genetic", 70 * 10) for instance in sorted(instances50)[:5]] * 25

    print(len(jobs))

    with open("exp2-genetic-%s.log" % datetime.now().strftime("%d_%H:%M"), "w") as fd:
        with multiprocessing.Pool(None) as pool:
            for res in pool.imap(star_run, jobs):
                fd.write(json.dumps(res))
                fd.write("\n")
                fd.flush()
