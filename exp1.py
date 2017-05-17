import multiprocessing
import subprocess
import glob
import json
from datetime import datetime


def runi(instance, algo, timeout=200):
    out = subprocess.check_output("./fssp %s %i %s" % (instance, timeout, algo), shell=True)
    lines = out.decode().strip().split("\n")
    initial = lines[0]
    times = lines[1:-1]
    times = list(map(lambda x: x.split(" "), times))
    times = list(map(lambda x: (float(x[0]), x[1]), times))
    end = lines[-1]

    return {
        "instance": instance,
        "algo": algo,
        "timeout": timeout,
        "initial": initial,
        "times": times,
        "score": end
    }


instances50 = glob.glob("instances/50_*_*")
instances100 = glob.glob("instances/100_*_*")


def star_run(args):
    try:
        return runi(*args)
    except Exception as e:
        return {"error": str(e), "args": args}


if __name__ == '__main__':
    jobs = [(instance, "ils", 70) for instance in instances50] * 5
    jobs += [(instance, "genetic", 70) for instance in instances50] * 5
    jobs += [(instance, "ils", 200) for instance in instances100] * 5
    jobs += [(instance, "genetic", 200) for instance in instances100] * 5

    print(len(jobs))

    with open("exp1-%s.log" % datetime.now().strftime("%d_%H:%M"), "w") as fd:
        with multiprocessing.Pool(None) as pool:
            for res in pool.imap(star_run, jobs):
                fd.write(json.dumps(res))
                fd.write("\n")
                fd.flush()
