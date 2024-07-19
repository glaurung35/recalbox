template = (
    "{{ d.name }}:"

    "{% if d.players == '1' %}"
        "player{{ d.players }}="
        '"'
        "{% for control in d.controls %}"
            "control={{ control.type }}{% if control.ways %}{{ control.ways }}{% endif %}{% if control.buttons %},buttons={{ control.buttons }}{% endif %}"
            "{% if not loop.last %}"
                "|"
            "{% endif %}"
        "{% endfor %}"
        '"'
    "{% else %}"
        "{% set count = 1 %}"
        "{% for control in d.controls %}"
            "{% if d.players|int == loop.length %}"
                "player{{ loop.index }}="
                '"'
                "control={{ control.type }}{% if control.ways %}{{ control.ways }}{% endif %}{% if control.buttons %},buttons={{ control.buttons }}{% endif %}"
                '"'
                "{% if not loop.last %},{% endif %}"
            "{% else %}"
                "{% if control.player|int == count and loop.first %}"
                    "player{{ count }}="
                    '"'
                    "control={{ control.type }}{% if control.ways %}{{ control.ways }}{% endif %}{% if control.buttons %},buttons={{ control.buttons }}{% endif %}"
                    "{% if not loop.last %}"
                        "|"
                    "{% endif %}"
                "{% elif control.player|int == count %}"
                    "control={{ control.type }}{% if control.ways %}{{ control.ways }}{% endif %}{% if control.buttons %},buttons={{ control.buttons }}{% endif %}"
                "{% else %}"
                    "{% set count = count + 1 %}"
                    '"'
                    ",player{{ count }}="
                    '"'
                    "control={{ control.type }}{% if control.ways %}{{ control.ways }}{% endif %}{% if control.buttons %},buttons={{ control.buttons }}{% endif %}"
                "{% endif %}"
                "{% if loop.last %}"
                    '"'
                "{% endif %}"
            "{% endif %}"
        "{% endfor %}"
    "{% endif %}"
)
